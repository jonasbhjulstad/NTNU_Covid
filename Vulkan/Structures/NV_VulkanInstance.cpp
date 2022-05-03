#include "NV_VulkanInstance.hpp"

void destroyVulkanInstance(VulkanInstance& vI)
{
	// Clean up Vulkan resources
	VkDevice logicalDevice = vI.vulkanDevice->logicalDevice;
	if (vI.descriptorPool != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorPool(logicalDevice, vI.descriptorPool, nullptr);
	}
	initializers::destroyCommandBuffers(logicalDevice, vI.vulkanDevice->commandPool, vI.drawCmdBuffers);
	if (vI.renderPass != VK_NULL_HANDLE)
	{
		vkDestroyRenderPass(logicalDevice, vI.renderPass, nullptr);
	}
	for (uint32_t i = 0; i < vI.frameBuffers.size(); i++)
	{
		vkDestroyFramebuffer(logicalDevice, vI.frameBuffers[i], nullptr);
	}

	for (auto& shaderModule : vI.shaderModules)
	{
		vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
	}
	vkDestroyImageView(logicalDevice, vI.depthStencil.view, nullptr);
	vkDestroyImage(logicalDevice, vI.depthStencil.image, nullptr);
	vkFreeMemory(logicalDevice, vI.depthStencil.mem, nullptr);

	vkDestroyPipelineCache(logicalDevice, vI.pipelineCache, nullptr);

	vkDestroyCommandPool(logicalDevice, vI.vulkanDevice->commandPool, nullptr);

	vkDestroySemaphore(logicalDevice, vI.semaphores.presentComplete, nullptr);
	vkDestroySemaphore(logicalDevice, vI.semaphores.renderComplete, nullptr);
	for (auto& fence : vI.waitFences) {
		vkDestroyFence(logicalDevice, fence, nullptr);
	}

	delete vI.vulkanDevice;

	// if (settings.validation)
	// {
	// 	vks::debug::freeDebugCallback(instance);
	// }

	vkDestroyInstance(vI.instance, nullptr);

    
    
}

void ImGui_Vulkan_Init(const VulkanInstance &vulkanInstance)
{
    ImGui_ImplGlfw_InitForVulkan(vulkanInstance.glfwWindow, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = vulkanInstance.instance;
    init_info.PhysicalDevice = vulkanInstance.vulkanDevice->physicalDevice;
    init_info.Device = vulkanInstance.vulkanDevice->logicalDevice;
    init_info.QueueFamily = vulkanInstance.vulkanDevice->queueFamilyIndices.graphics;
    init_info.Queue = vulkanInstance.queue;
    init_info.PipelineCache = vulkanInstance.pipelineCache;
    init_info.DescriptorPool = vulkanInstance.descriptorPool;
    init_info.Allocator = NULL;
    init_info.MinImageCount = 2;
    init_info.ImageCount = vulkanInstance.swapChain.imageCount;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, vulkanInstance.renderPass);
}