#ifndef NV_VULKAN_WINDOW_HPP
#define NV_VULKAN_WINDOW_HPP
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>
#include <NV_VulkanInstance.hpp>
#include <NV_VulkanPipelineInitializers.hpp>
#include <NV_VulkanInitializers.hpp>



void setupGLFWVulkanWindow(
                       VulkanInstance &vulkanInstance,
                       int width,
                       int height,
                       uint32_t minImageCount = 2);
void cleanupGLFWVulkanWindow(VkInstance instance, VkDevice device, ImGui_ImplVulkanH_Window *wd);

#endif // NV_VULKAN_WINDOW_HPP