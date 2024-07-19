#include <VulkanViewport/Window/Resize.hpp>
#include <imgui/imgui_impl_vulkan.h>
namespace VkVP {
static void recreateWindowBuffers(VulkanInstance &vulkanInstance,
                                  uint32_t width, uint32_t height) {
  auto &logicalDevice = vulkanInstance.vulkanDevice->logicalDevice;
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
}

void resizeImGuiWindow(VulkanInstance &vulkanInstance, uint32_t width,
                       uint32_t height) {

  // Imgui global variables
  ImGui_ImplVulkan_SetMinImageCount(vulkanInstance.swapChain.imageCount);
  ImGui_ImplVulkanH_CreateOrResizeWindow(
      vulkanInstance.instance, vulkanInstance.vulkanDevice->physicalDevice,
      vulkanInstance.vulkanDevice->logicalDevice, &vulkanInstance.ImGuiWindow,
      vulkanInstance.vulkanDevice->queueFamilyIndices.graphics, nullptr, width,
      height, vulkanInstance.swapChain.imageCount);
  vulkanInstance.ImGuiWindow.FrameIndex = 0;

  // VulkanInstance-related variables
  vkDeviceWaitIdle(logicalDevice);
  vulkanInstance.swapChain.create(&vulkanInstance.ImGuiWindow,
                                  (uint32_t *)&width, (uint32_t *)&height,
                                  false);
  vulkanInstance.ImGuiWindow.Swapchain = vulkanInstance.swapChain.swapChain;

  recreateWindowBuffers(vulkanInstance, width, height);
}

} // namespace VkVP