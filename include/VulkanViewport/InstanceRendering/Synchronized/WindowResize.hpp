#pragma once

#include <VulkanTools/Instance.hpp>
#include <VulkanTools/InstanceRendering/InstancePipeline.hpp>
#include <VulkanTools/Renderpass.hpp>
#include <VulkanTools/Tools.hpp>
#include <VulkanViewport/UI/ImGuiUI.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace VkVP {
void updateWindowSize(
    VulkanInstance &vulkanInstance, VkVP::ImGuiVulkanData &ivData,
    Camera &camera,
    const std::vector<std::unique_ptr<VkVP::InstancePipelineData>>
        &instancePipelines,
    int &width, int &height) {
  glfwGetFramebufferSize(vulkanInstance.glfwWindow, &width, &height);
  if (width_old != width || height_old != height) {
    rebuildBuffers(vulkanInstance, instancePipelines, ivData, camera, width,
                   height);
  }
  width_old = width;
  height_old = height;
}
} // namespace VkVP