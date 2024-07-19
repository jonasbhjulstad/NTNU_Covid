#pragma once
#include <VulkanTools/Instance.hpp>
#include <VulkanTools/InstanceRendering/InstancePipeline.hpp>
#include <VulkanTools/Tools.hpp>
#include <VulkanTools/Renderpass.hpp>
#include <VulkanViewport/UI/ImGuiUI.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>
namespace VkVP
{
void beginCommandBuffer(VkCommandBuffer commandBuffer) {
  VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
  VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &cmdBufInfo));
}

void buildCommandBuffers(
    std::vector<VkCommandBuffer> &commandBuffers,
    std::vector<VkFramebuffer> &frameBuffers, VkRenderPass renderPass,
    VkVP::ImGuiVulkanData &ivData,
    const std::vector<std::unique_ptr<VkVP::InstancePipelineData>>
        &instancePipelines,
    int width, int height) {
  // VkCommandBufferBeginInfo cmdBufInfo =
  // initializers::commandBufferBeginInfo();
  for (int32_t i = 0; i < commandBuffers.size(); ++i) {

    // VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer[i], &cmdBufInfo));
    beginCommandBuffer(commandBuffers[i]);

    beginRenderPass(renderPass, commandBuffers[i], frameBuffers[i], width,
                    height);
    for (auto &instancePipeline : instancePipelines) {
      buildCommandBuffer(*instancePipeline, commandBuffers[i]);
    }
    VkVP::drawFrame(ivData, commandBuffers[i]);

    vkCmdEndRenderPass(commandBuffers[i]);

    VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffers[i]));
  }
}


void rebuildBuffers(
    VulkanInstance &vulkanInstance,
    const std::vector<std::unique_ptr<VkVP::InstancePipelineData>>
        &instancePipelines,
    VkVP::ImGuiVulkanData &ivData, Camera &camera, int width, int height) {
  VkDevice logicalDevice = vulkanInstance.vulkanDevice->logicalDevice;
  // Ensure all operations on the device have been finished before destroying
  // resources
  vkDeviceWaitIdle(logicalDevice);
  vulkanInstance.swapChain.create(&vulkanInstance.ImGuiWindow,
                                  (uint32_t *)&width, (uint32_t *)&height,
                                  false);
  vulkanInstance.ImGuiWindow.Swapchain = vulkanInstance.swapChain.swapChain;
  // Recreate the frame buffersqueuePresent
  vkDestroyImageView(logicalDevice, vulkanInstance.depthStencil.view, nullptr);
  vkDestroyImage(logicalDevice, vulkanInstance.depthStencil.image, nullptr);
  vkFreeMemory(logicalDevice, vulkanInstance.depthStencil.mem, nullptr);
  initializers::setupDepthStencil(vulkanInstance, width, height);
  for (uint32_t i = 0; i < vulkanInstance.frameBuffers.size(); i++) {
    vkDestroyFramebuffer(logicalDevice, vulkanInstance.frameBuffers[i],
                         nullptr);
  }
  initializers::setupFrameBuffer(
      logicalDevice, vulkanInstance.renderPass, width, height,
      vulkanInstance.depthStencil.view, vulkanInstance.swapChain,
      vulkanInstance.frameBuffers);

  // Command buffers need to be recreated as they may store
  // references to the recreated frame buffer
  initializers::destroyCommandBuffers(logicalDevice,
                                      vulkanInstance.vulkanDevice->commandPool,
                                      vulkanInstance.drawCmdBuffers);
  initializers::createCommandBuffers(
      logicalDevice, vulkanInstance.drawCmdBuffers, vulkanInstance.swapChain,
      vulkanInstance.vulkanDevice->commandPool);

  buildCommandBuffers(vulkanInstance.drawCmdBuffers,
                      vulkanInstance.frameBuffers, vulkanInstance.renderPass,
                      ivData, instancePipelines, width, height);

  vkDeviceWaitIdle(logicalDevice);

  if ((width > 0.0f) && (height > 0.0f)) {
    camera.updateAspectRatio((float)width / (float)height);
  }
}

void submitBuffers(VulkanInstance &vulkanInstance, uint32_t &currentBufferIdx) {
  VK_CHECK_RESULT(vulkanInstance.swapChain.acquireNextImage(
      vulkanInstance.semaphores.presentComplete, &currentBufferIdx));
  vulkanInstance.submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  vulkanInstance.submitInfo.commandBufferCount = 1;
  vulkanInstance.submitInfo.pCommandBuffers =
      &vulkanInstance.drawCmdBuffers[currentBufferIdx];
  vulkanInstance.submitInfo.pNext = nullptr;
  VK_CHECK_RESULT(vkQueueSubmit(vulkanInstance.queue, 1,
                                &vulkanInstance.submitInfo, VK_NULL_HANDLE));
  VK_CHECK_RESULT(vulkanInstance.swapChain.queuePresent(
      vulkanInstance.queue, currentBufferIdx,
      vulkanInstance.semaphores.renderComplete));
  VK_CHECK_RESULT(vkQueueWaitIdle(vulkanInstance.queue));
}
}  // namespace VkVP