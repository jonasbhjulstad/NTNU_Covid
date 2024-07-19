#ifndef SETUP_ROUTINES_HPP
#define SETUP_ROUTINES_HPP
#include <VulkanTools/Instance.hpp>
#include <VulkanTools/InstanceRendering/InstancePipeline.hpp>
#include <VulkanTools/Tools.hpp>
#include <VulkanTools/Renderpass.hpp>
#include <VulkanViewport/UI/ImGuiUI.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>





void updateWindowSize(
    VulkanInstance &vulkanInstance, VkVP::ImGuiVulkanData &ivData,
    Camera &camera,
    const std::vector<std::unique_ptr<VkVP::InstancePipelineData>>
        &instancePipelines,
    int &width, int &height) {
  static int width_old, height_old;
  // glfwGetWindowSize(vulkanInstance.glfwWindow, &width, &height);
  glfwGetFramebufferSize(vulkanInstance.glfwWindow, &width, &height);
  if (width_old != width || height_old != height) {
    ImGui_ImplVulkan_SetMinImageCount(vulkanInstance.swapChain.imageCount);
    ImGui_ImplVulkanH_CreateOrResizeWindow(
        vulkanInstance.instance, vulkanInstance.vulkanDevice->physicalDevice,
        vulkanInstance.vulkanDevice->logicalDevice, &vulkanInstance.ImGuiWindow,
        vulkanInstance.vulkanDevice->queueFamilyIndices.graphics, nullptr,
        width, height, vulkanInstance.swapChain.imageCount);
    vulkanInstance.ImGuiWindow.FrameIndex = 0;
    rebuildBuffers(vulkanInstance, instancePipelines, ivData, camera, width,
                   height);
  }
  width_old = width;
  height_old = height;
}



#endif