#ifndef NV_VULKAN_WINDOW_HPP
#define NV_VULKAN_WINDOW_HPP
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_vulkan.h>
#include "NV_VulkanInstance.hpp"



void SetupGLFWVulkanWindow(ImGui_ImplVulkanH_Window *wd,
                       VkSurfaceKHR surface,
                       const VulkanInstance &vulkanInstance,
                       int width,
                       int height,
                       uint32_t minImageCount = 2);
void cleanupGLFWVulkanWindow(VkInstance instance, VkDevice device, ImGui_ImplVulkanH_Window *wd);

#endif // NV_VULKAN_WINDOW_HPP