#include "NV_VulkanWindow.hpp"
void setupGLFWVulkanWindow(VulkanInstance &vulkanInstance,
                       int width,
                       int height,
                       uint32_t minImageCount)
{
    ImGui_ImplVulkanH_Window* wd = &vulkanInstance.ImGuiWindow;
    wd->Surface = vulkanInstance.surface;
    VkPhysicalDevice physicalDevice = vulkanInstance.vulkanDevice->physicalDevice;
    VkDevice logicalDevice = vulkanInstance.vulkanDevice->logicalDevice;
    uint32_t queueFamily = vulkanInstance.vulkanDevice->queueFamilyIndices.graphics;
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
    wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(physicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

    VkPresentModeKHR present_modes[] = {VK_PRESENT_MODE_FIFO_KHR};
    wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(physicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
    // printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

    ImGui_ImplVulkanH_CreateOrResizeWindow(vulkanInstance.instance, physicalDevice, logicalDevice, wd, queueFamily, nullptr, width, height, minImageCount);
}

void cleanupGLFWVulkanWindow(VkInstance instance, VkDevice device, ImGui_ImplVulkanH_Window *wd)
{
    ImGui_ImplVulkanH_DestroyWindow(instance, device, wd, NULL);
}