#pragma once
#include <VulkanTools/Instance.hpp>
#include <VulkanTools/InstanceRendering/InstancePipeline.hpp>
#include <VulkanTools/Renderpass.hpp>
#include <VulkanTools/Tools.hpp>
#include <VulkanViewport/UI/ImGuiUI.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>
namespace VkVP {




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
} // namespace VkVP