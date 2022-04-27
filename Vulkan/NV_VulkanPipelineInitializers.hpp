#ifndef NV_VULKAN_PIPELINE_INITIALIZERS_HPP
#define NV_VULKAN_PIPELINE_INITIALIZERS_HPP
#include <vulkan/vulkan.hpp>
#include "NV_Vulkan.hpp"
#include "NV_VulkanInstance.hpp"
namespace initializers
{
	void createCommandBuffers(VkDevice device, std::vector<VkCommandBuffer> &drawCmdBuffers, VulkanSwapChain &swapChain, VkCommandPool cmdPool);

	void createCommandPool(VkDevice device, VulkanSwapChain &swapChain, VkCommandPool cmdPool);

	void createWaitFences(VkDevice device, std::vector<VkCommandBuffer> &drawCmdBuffers, std::vector<VkFence> &waitFences);

	void setupDepthStencil(VulkanInstance &vulkanInstance, uint32_t width, uint32_t height);

	void setupRenderPass(VkDevice device, VulkanSwapChain &swapChain, VkRenderPass& renderPass, VkFormat depthFormat);

	void createPipelineCache(VkDevice device, VkPipelineCache* pipelineCache);

	void setupFrameBuffer(VkDevice device,
						  VkRenderPass renderPass,
						  uint32_t width,
						  uint32_t height,
						  VkImageView view,
						  VulkanSwapChain &swapChain,
						  std::vector<VkFramebuffer> &frameBuffers);

}

#endif