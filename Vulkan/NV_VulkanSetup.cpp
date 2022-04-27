#include "NV_VulkanSetup.hpp"
VkResult createVulkanInstance(bool enableValidation,
							  const std::string &name,
							  VkInstance& instance,
							  std::vector<std::string> &supportedInstanceExtensions,
							  std::vector<const char *> &enabledInstanceExtensions,
							  uint32_t apiVersion)
{

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = name.c_str();
	appInfo.pEngineName = name.c_str();
	appInfo.apiVersion = apiVersion;

	std::vector<const char *> instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME};

	// Get extensions supported by the instance and store for later use
	uint32_t extCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
	if (extCount > 0)
	{
		std::vector<VkExtensionProperties> extensions(extCount);
		if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
		{
			for (VkExtensionProperties extension : extensions)
			{
				supportedInstanceExtensions.push_back(extension.extensionName);
			}
		}
	}

	// Enabled requested instance extensions
	if (enabledInstanceExtensions.size() > 0)
	{
		for (const char *enabledExtension : enabledInstanceExtensions)
		{
			// Output message if requested extension is not available
			if (std::find(supportedInstanceExtensions.begin(), supportedInstanceExtensions.end(), enabledExtension) == supportedInstanceExtensions.end())
			{
				std::cerr << "Enabled instance extension \"" << enabledExtension << "\" is not present at instance level\n";
			}
			instanceExtensions.push_back(enabledExtension);
		}
	}

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = NULL;
	instanceCreateInfo.pApplicationInfo = &appInfo;
	if (instanceExtensions.size() > 0)
	{
		if (enableValidation)
		{
			instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		instanceCreateInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
		instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
	}

	const char *validationLayerName = "VK_LAYER_KHRONOS_validation";
	if (enableValidation)
	{
		// Check if this layer is available at instance level
		uint32_t instanceLayerCount;
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
		std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
		bool validationLayerPresent = false;
		for (VkLayerProperties layer : instanceLayerProperties)
		{
			if (strcmp(layer.layerName, validationLayerName) == 0)
			{
				validationLayerPresent = true;
				break;
			}
		}
		if (validationLayerPresent)
		{
			instanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
			instanceCreateInfo.enabledLayerCount = 1;
		}
		else
		{
			std::cerr << "Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled";
		}
	}
	return vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
}

void setupVulkanPhysicalDevice(VulkanInstance &vulkanInstance, bool enableValidation)
{
	// If requested, we enable the default validation layers for debugging
	if (enableValidation)
	{
		// The report flags determine what type of messages for the layers will be displayed
		// For validating (debugging) an application the error and warning bits should suffice
		VkDebugReportFlagsEXT debugReportFlags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
		// Additional flags include performance info, loader and layer debug messages, etc.
		debug::setupVulkanDebug(vulkanInstance.instance, debugReportFlags, VK_NULL_HANDLE);
	}


	// Physical device
	uint32_t gpuCount = 0;
	// Get number of available physical devices
	VK_CHECK_RESULT(vkEnumeratePhysicalDevices(vulkanInstance.instance, &gpuCount, nullptr));
	if (gpuCount == 0)
	{
		std::cerr << "Failed to find GPUs with Vulkan support." << std::endl;
	}
	
	// Enumerate devices
	std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
	if (vkEnumeratePhysicalDevices(vulkanInstance.instance, &gpuCount, physicalDevices.data()) != VK_SUCCESS)
	{
		std::cerr << "Failed to find GPUs with Vulkan support." << std::endl;
	}

	// GPU selection

	// Select physical device to be used for the Vulkan example
	// Defaults to the first device unless specified by command line
	uint32_t selectedDevice = 0;

	VkPhysicalDevice physicalDevice = physicalDevices[selectedDevice];

	vulkanInstance.vulkanDevice = new VulkanDevice(physicalDevice);
	VulkanDevice* vulkanDevice = vulkanInstance.vulkanDevice;

	// Store properties (including limits), features and memory properties of the physical device (so that examples can check against them)
	vkGetPhysicalDeviceProperties(physicalDevice, &vulkanDevice->properties);
	vkGetPhysicalDeviceFeatures(physicalDevice, &vulkanDevice->features);
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &vulkanDevice->memoryProperties);

	// Vulkan device creation
	// This is handled by a separate class that gets a logical device representation
	// and encapsulates functions related to a device
	if (vulkanDevice->createLogicalDevice(vulkanInstance.enabledFeatures, vulkanInstance.enabledDeviceExtensions, nullptr) != VK_SUCCESS)
	{
		std::cerr << "Failed to create logical device!" << std::endl;
	}

	VkDevice device = vulkanDevice->logicalDevice;

	// Get a graphics queue from the device
	vkGetDeviceQueue(device, vulkanDevice->queueFamilyIndices.graphics, 0, &vulkanInstance.queue);

	// Find a suitable depth format
	VkBool32 validDepthFormat = getSupportedDepthFormat(physicalDevice, &vulkanInstance.depthFormat);
	assert(validDepthFormat);

	vulkanInstance.swapChain.connect(vulkanInstance.instance, physicalDevice, device);

	// Create synchronization objects
	VkSemaphoreCreateInfo semaphoreCreateInfo = initializers::semaphoreCreateInfo();
	// Create a semaphore used to synchronize image presentation
	// Ensures that the image is displayed before we start submitting new commands to the queue
	VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &vulkanInstance.semaphores.presentComplete));
	// Create a semaphore used to synchronize command submission
	// Ensures that the image is not presented until all commands have been submitted and executed
	VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &vulkanInstance.semaphores.renderComplete));

	// Set up submit info structure
	// Semaphores will stay the same during application lifetime
	// Command buffer submission info is set by each example
	vulkanInstance.submitInfo.pWaitDstStageMask = &vulkanInstance.submitPipelineStages;
	vulkanInstance.submitInfo.waitSemaphoreCount = 1;
	vulkanInstance.submitInfo.pWaitSemaphores = &vulkanInstance.semaphores.presentComplete;
	vulkanInstance.submitInfo.signalSemaphoreCount = 1;
	vulkanInstance.submitInfo.pSignalSemaphores = &vulkanInstance.semaphores.renderComplete;
}


void prepareVulkan(VulkanInstance &vulkanInstance, uint32_t width, uint32_t height)
{
	VkDevice logicalDevice = vulkanInstance.vulkanDevice->logicalDevice;
	vulkanInstance.swapChain.initSurface(vulkanInstance.instance, vulkanInstance.glfwWindow, vulkanInstance.surface, width, height);
	initializers::createCommandPool(logicalDevice, vulkanInstance.swapChain, vulkanInstance.vulkanDevice->commandPool);
	vulkanInstance.swapChain.create(&vulkanInstance.ImGuiWindow, &width, &height);
	initializers::createCommandBuffers(logicalDevice, vulkanInstance.drawCmdBuffers, vulkanInstance.swapChain, vulkanInstance.vulkanDevice->commandPool);
	initializers::createWaitFences(logicalDevice, vulkanInstance.drawCmdBuffers, vulkanInstance.waitFences);
	initializers::setupDepthStencil(vulkanInstance, width, height);
	initializers::setupRenderPass(logicalDevice, vulkanInstance.swapChain, vulkanInstance.renderPass,vulkanInstance.depthFormat);
	initializers::createPipelineCache(logicalDevice, vulkanInstance.pipelineCache);
	initializers::setupFrameBuffer(logicalDevice, 
	vulkanInstance.renderPass, 
	width, 
	height, 
	vulkanInstance.depthStencil.view, 
	vulkanInstance.swapChain, 
	vulkanInstance.frameBuffers);
}
