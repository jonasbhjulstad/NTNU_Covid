#define KTX_OPENGL_ES3 1

#include <imgui.h>
// #include "VulkanglTFModel.h"
#include <random>
#include <chrono>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#define ENABLE_VALIDATION true
#include "NV_VulkanSetup.hpp"

#include "NV_Node.hpp"
#include "NV_Edge.hpp"
#include <igraph/igraph.h>
#include <igraph/igraph_games.h>
#include <igraph/igraph_layout.h>
#include <igraph/igraph_epidemics.h>


int main()
{
    VulkanInstance vulkanInstance;
        // Setup GLFW window
    if (!glfwInit())
        return 1;

    const uint32_t width = 1280;
    const uint32_t height = 720;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(width, height, "Network Viewport", NULL, NULL);

    // Setup Vulkan
    if (!glfwVulkanSupported())
    {
        printf("GLFW: Vulkan Not Supported\n");
        return 1;
    }
    uint32_t extensions_count = 0;
    const char** extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
    for (int i = 0; i < extensions_count; i++)
    {
        vulkanInstance.enabledDeviceExtensions.push_back(extensions[i]);
    }
    createVulkanInstance(ENABLE_VALIDATION, "Network Viewport", vulkanInstance.instance, vulkanInstance.supportedInstanceExtensions, vulkanInstance.enabledInstanceExtensions, VK_API_VERSION_1_0);
    setupVulkanPhysicalDevice(vulkanInstance, ENABLE_VALIDATION);

    // Create Window Surface
    VkSurfaceKHR surface;
    VkResult err = glfwCreateWindowSurface(vulkanInstance.instance, window, nullptr, &surface);

    prepareVulkan(vulkanInstance, width, height);

}