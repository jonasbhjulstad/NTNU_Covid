#define KTX_OPENGL_ES3 1

// #include "VulkanglTFModel.h"
#include <random>
#include <chrono>
#include <vulkan/vulkan.hpp>
#include <imgui.h>
#include <GLFW/glfw3.h>
#define ENABLE_VALIDATION true
#include <NV_VulkanSetup.hpp>
#include <NV_VulkanWindow.hpp>

#include "NV_Node.hpp"
#include "NV_Edge.hpp"
#include <igraph/igraph.h>
#include <igraph/igraph_games.h>
#include <igraph/igraph_layout.h>
#include <igraph/igraph_epidemics.h>

static void check_vk_result(VkResult err)
{
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}

int main()
{
    VulkanInstance vulkanInstance;
    VulkanDevice *vulkanDevice = vulkanInstance.vulkanDevice;
    // Setup GLFW window
    if (!glfwInit())
        return 1;

    const uint32_t width = 1280;
    const uint32_t height = 720;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    vulkanInstance.glfwWindow = glfwCreateWindow(width, height, "Network Viewport", NULL, NULL);

    // Setup Vulkan
    if (!glfwVulkanSupported())
    {
        printf("GLFW: Vulkan Not Supported\n");
        return 1;
    }
    uint32_t extensions_count = 0;
    const char **extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
    for (int i = 0; i < extensions_count; i++)
    {
        vulkanInstance.enabledInstanceExtensions.push_back(extensions[i]);
    }
    createVulkanInstance(ENABLE_VALIDATION, "Network Viewport", vulkanInstance.instance, vulkanInstance.supportedInstanceExtensions, vulkanInstance.enabledInstanceExtensions, VK_API_VERSION_1_0);
    setupVulkanPhysicalDevice(vulkanInstance, ENABLE_VALIDATION);

    // TODO: glfw-surface
    VK_CHECK_RESULT(glfwCreateWindowSurface(vulkanInstance.instance, vulkanInstance.glfwWindow, NULL, &vulkanInstance.surface));

    setupGLFWVulkanWindow(vulkanInstance, width, height, 2);
    prepareVulkan(vulkanInstance, width, height);

    // Descriptor pool
    std::vector<VkDescriptorPoolSize> poolSizes = {
        initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)};
    VkDescriptorPoolCreateInfo descriptorPoolInfo = initializers::descriptorPoolCreateInfo(poolSizes, 2);
    VK_CHECK_RESULT(vkCreateDescriptorPool(vulkanDevice->logicalDevice, &descriptorPoolInfo, nullptr, &vulkanInstance.descriptorPool));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForVulkan(vulkanInstance.glfwWindow, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = vulkanInstance.instance;
    init_info.PhysicalDevice = vulkanInstance.vulkanDevice->physicalDevice;
    init_info.Device = vulkanInstance.vulkanDevice->logicalDevice;
    init_info.QueueFamily = vulkanInstance.vulkanDevice->queueFamilyIndices.graphics;
    init_info.Queue = vulkanInstance.queue;
    VkPipelineCache pipelineCache;
    init_info.PipelineCache = pipelineCache;
    init_info.DescriptorPool = vulkanInstance.descriptorPool;
    init_info.Allocator = NULL;
    init_info.MinImageCount = 2;
    init_info.ImageCount = vulkanInstance.swapChain.imageCount;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, vulkanInstance.renderPass);
}