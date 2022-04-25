#ifndef NV_VULKAN_WINDOW_HPP
#define NV_VULKAN_WINDOW_HPP
#include <GLFW/glfw3.h>
#include <imgui/imgui_impl_glfw.h>
#include "NV_VulkanInstance.hpp"
void SetupGLFWVulkanWindow(ImGui_ImplVulkanH_Window *wd,
                       VkSurfaceKHR surface,
                       const VulkanInstance &vulkanInstance,
                       int width,
                       int height,
                       uint32_t minImageCount = 2)
{
    wd->Surface = surface;
    VkPhysicalDevice physicalDevice = vulkanInstance.physicalDevice;
    VkLogicalDevice logicalDevice = vulkanInstance.logicalDevice;
    uint32_t queueFamily = vulkanInstance.queueFamily;
    // Check for WSI support
    VkBool32 res;
    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamily, wd->Surface, &res);
    if (res != VK_TRUE)
    {
        fprintf(stderr, "Error no WSI support on physical device 0\n");
        exit(-1);
    }

    // Select Surface Format
    const VkFormat requestSurfaceImageFormat[] = {VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM};
    const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(g_PhysicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

    VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_FIFO_KHR};
    wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(g_PhysicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
    // printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

    ImGui_ImplVulkanH_CreateOrResizeWindow(vulkanInstance.instance, physicalDevice, logicalDevice, wd, queueFamily, nullptr, width, height, minImageCount);
}

void cleanupGLFWVulkanWindow(ImGui_ImplVulkanH_Window *wd)
{
    ImGui_ImplVulkanH_DestroyWindow(wd);
}

#endif // NV_VULKAN_WINDOW_HPP