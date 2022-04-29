#define KTX_OPENGL_ES3 1
#define ENABLE_VALIDATION true

// #include "VulkanglTFModel.h"
#include <random>
#include <chrono>
#include <vulkan/vulkan.hpp>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <NV_VulkanSetup.hpp>
#include <NV_VulkanWindow.hpp>

#include "NV_Node.hpp"
#include "NV_Edge.hpp"
#include <igraph/igraph.h>
#include <igraph/igraph_games.h>
#include <igraph/igraph_layout.h>
#include <igraph/igraph_epidemics.h>

#include "ImGuiApp.hpp"
#include "NV_UISettings.hpp"

#ifdef WIN32
	const std::string assetPath = "C:\\Users\\jonas\\Documents\\Vulkan\\examples\\imgui\\data\\";
	const std::string shadersPath = assetPath + "shaders\\";
    const std::string computeShadersPath = assetPath + "computeShaders";
	const std::string modelPath = assetPath + "models\\";
	const std::string texturePath = assetPath + "textures\\";
#else
	const std::string assetPath = "/home/deb/Documents/Vulkan/examples/imgui/data/";
	const std::string shadersPath = assetPath + "shaders/";
    const std::string computeShadersPath = assetPath + "computeShaders";
	const std::string modelPath = assetPath + "models/";
	const std::string texturePath = assetPath + "textures/";
#endif


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
    UISettings uiSettings;

    uiSettings.fontPath = assetPath + "fonts/DroidSansMono.ttf";
    VulkanInstance vulkanInstance;
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

    VulkanDevice *vulkanDevice = vulkanInstance.vulkanDevice;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForVulkan(vulkanInstance.glfwWindow, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = vulkanInstance.instance;
    init_info.PhysicalDevice = vulkanInstance.vulkanDevice->physicalDevice;
    init_info.Device = vulkanInstance.vulkanDevice->logicalDevice;
    init_info.QueueFamily = vulkanInstance.vulkanDevice->queueFamilyIndices.graphics;
    init_info.Queue = vulkanInstance.queue;
    init_info.PipelineCache = vulkanInstance.pipelineCache;
    init_info.DescriptorPool = vulkanInstance.descriptorPool;
    init_info.Allocator = NULL;
    init_info.MinImageCount = 2;
    init_info.ImageCount = vulkanInstance.swapChain.imageCount;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, vulkanInstance.renderPass);

    ImGUI App(vulkanDevice, "Network Viewport");
    App.init(width, height, uiSettings);

    App.initResources(vulkanInstance.renderPass, vulkanInstance.queue, shadersPath);
    bool rebuildSwapChain = false;
    
    while(!glfwWindowShouldClose(vulkanInstance.glfwWindow))
    {
                // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Resize swap chain?
        if (rebuildSwapChain)
        {
            int width, height;
            glfwGetFramebufferSize(vulkanInstance.glfwWindow, &width, &height);
            if (width > 0 && height > 0)
            {
                ImGui_ImplVulkan_SetMinImageCount(vulkanInstance.swapChain.imageCount);
                ImGui_ImplVulkanH_CreateOrResizeWindow(vulkanInstance.instance, vulkanDevice->physicalDevice, vulkanDevice->logicalDevice, &vulkanInstance.ImGuiWindow, vulkanInstance.queue, NULL, width, height, vulkanInstance.swapChain.imageCount);
                vulkanInstance.ImGuiWindow.FrameIndex = 0;
                rebuildSwapChain = false;
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        App.newFrame(1, uiSettings, frameTime, );
    }

    ImGui_ImplVulkanH_DestroyWindow(vulkanInstance.instance, vulkanDevice->logicalDevice, &vulkanInstance.ImGuiWindow, NULL);
    vkDestroyDescriptorPool(vulkanDevice->logicalDevice, vulkanInstance.descriptorPool, NULL);

#ifdef IMGUI_VULKAN_DEBUG_REPORT
    // Remove the debug report callback
    auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkDestroyDebugReportCallbackEXT");
    vkDestroyDebugReportCallbackEXT(g_Instance, g_DebugReport, g_Allocator);
#endif // IMGUI_VULKAN_DEBUG_REPORT

    vkDestroyDevice(vulkanDevice->logicalDevice, NULL);
    vkDestroyInstance(vulkanInstance.instance, NULL);

    glfwDestroyWindow(vulkanInstance.glfwWindow);
    glfwTerminate();

    return 0;
}