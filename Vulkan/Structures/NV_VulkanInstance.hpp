#ifndef NV_VULKAN_INSTANCE_HPP
#define NV_VULKAN_INSTANCE_HPP
#include <vector>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>
#include "NV_VulkanDevice.hpp"
#include "NV_VulkanSwapChain.hpp"
#include "NV_VulkanTools.hpp"

struct VulkanInstance
{
    /** @brief Set of device extensions to be enabled for this example (must be set in the derived constructor) */
	std::vector<const char*> enabledDeviceExtensions;
	std::vector<const char*> enabledInstanceExtensions;


    // Handle to the device graphics queue that command buffers are submitted to
	VkQueue queue;
	// Depth buffer format (selected during Vulkan initialization)
	VkFormat depthFormat;

	VulkanDevice* vulkanDevice;

    /** @brief Pipeline stages used to wait at for graphics queue submissions */
	VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	// Contains command buffers and semaphores to be presented to the queue
	VkSubmitInfo submitInfo;
	
	// Command buffers used for rendering
	std::vector<VkCommandBuffer> drawCmdBuffers;
	// Global render pass for frame buffer writes
	VkRenderPass renderPass = VK_NULL_HANDLE;
	// List of available frame buffers (same as number of swap chain images)
	std::vector<VkFramebuffer>frameBuffers;
	// Active frame buffer index
	uint32_t currentBuffer = 0;
	// Descriptor set pool
	VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
	// List of shader modules created (stored for cleanup)
	std::vector<VkShaderModule> shaderModules;
	// Pipeline cache object
	VkPipelineCache pipelineCache;
	// Wraps the swap chain to present images (framebuffers) to the windowing system
	VulkanSwapChain swapChain;
	// Synchronization semaphores

	VkPhysicalDeviceFeatures enabledFeatures{};

    // Vulkan instance, stores all per-application states
	VkInstance instance = nullptr;
	std::vector<std::string> supportedInstanceExtensions;
	struct {
		// Swap chain image presentation
		VkSemaphore presentComplete;
		// Command buffer submission and execution
		VkSemaphore renderComplete;
	} semaphores;
	std::vector<VkFence> waitFences;
		struct {
		VkImage image;
		VkDeviceMemory mem;
		VkImageView view;
	} depthStencil;
	GLFWwindow* glfwWindow;
	ImGui_ImplVulkanH_Window ImGuiWindow;
	VkSurfaceKHR surface;
};

void destroyVulkanInstance(VulkanInstance& vulkanInstance);

void ImGui_Vulkan_Init(const VulkanInstance& vulkanInstance);
#endif