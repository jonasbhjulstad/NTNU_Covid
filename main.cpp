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
#include <NV_Camera.hpp>
#include <NV_ImGuiUI.hpp>
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


void buildCommandBuffers(VulkanInstance &vulkanInstance, ImGUI_UI::ImGuiVulkanData& ivData, int width, int height)
{
    VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();

    VkClearValue clearValues[2];
    clearValues[0].color = {{0.2f, 0.2f, 0.2f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    VkRenderPassBeginInfo renderPassBeginInfo = initializers::renderPassBeginInfo();
    renderPassBeginInfo.renderPass = vulkanInstance.renderPass;
    renderPassBeginInfo.renderArea.offset.x = 0;
    renderPassBeginInfo.renderArea.offset.y = 0;
    renderPassBeginInfo.renderArea.extent.width = width;
    renderPassBeginInfo.renderArea.extent.height = height;
    renderPassBeginInfo.clearValueCount = 2;
    renderPassBeginInfo.pClearValues = clearValues;
    for (int32_t i = 0; i < vulkanInstance.drawCmdBuffers.size(); ++i)
    {
        // Set target frame buffer
        renderPassBeginInfo.framebuffer = vulkanInstance.frameBuffers[i];

        VK_CHECK_RESULT(vkBeginCommandBuffer(vulkanInstance.drawCmdBuffers[i], &cmdBufInfo));

        vkCmdBeginRenderPass(vulkanInstance.drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport = initializers::viewport((float)width, (float)height, 0.0f, 1.0f);
        vkCmdSetViewport(vulkanInstance.drawCmdBuffers[i], 0, 1, &viewport);

        VkRect2D scissor = initializers::rect2D(width, height, 0, 0);
        vkCmdSetScissor(vulkanInstance.drawCmdBuffers[i], 0, 1, &scissor);


        ImGUI_UI::drawFrame(ivData, vulkanInstance.drawCmdBuffers[i]);

        vkCmdEndRenderPass(vulkanInstance.drawCmdBuffers[i]);

        VK_CHECK_RESULT(vkEndCommandBuffer(vulkanInstance.drawCmdBuffers[i]));
    }
}

void rebuildBuffers(VulkanInstance &vulkanInstance, ImGUI_UI::ImGuiVulkanData& ivData, Camera &camera, int width, int height)
{
    VkDevice logicalDevice = vulkanInstance.vulkanDevice->logicalDevice;
    // Ensure all operations on the device have been finished before destroying resources
    vkDeviceWaitIdle(logicalDevice);
    vulkanInstance.swapChain.create(&vulkanInstance.ImGuiWindow, (uint32_t *)&width, (uint32_t *)&height, false);
    vulkanInstance.ImGuiWindow.Swapchain = vulkanInstance.swapChain.swapChain;
    // Recreate the frame buffersqueuePresent
    vkDestroyImageView(logicalDevice, vulkanInstance.depthStencil.view, nullptr);
    vkDestroyImage(logicalDevice, vulkanInstance.depthStencil.image, nullptr);
    vkFreeMemory(logicalDevice, vulkanInstance.depthStencil.mem, nullptr);
    initializers::setupDepthStencil(vulkanInstance, width, height);
    for (uint32_t i = 0; i < vulkanInstance.frameBuffers.size(); i++)
    {
        vkDestroyFramebuffer(logicalDevice, vulkanInstance.frameBuffers[i], nullptr);
    }
    initializers::setupFrameBuffer(logicalDevice,
                                   vulkanInstance.renderPass,
                                   width,
                                   height,
                                   vulkanInstance.depthStencil.view,
                                   vulkanInstance.swapChain,
                                   vulkanInstance.frameBuffers);

    // Command buffers need to be recreated as they may store
    // references to the recreated frame buffer
    initializers::destroyCommandBuffers(logicalDevice, vulkanInstance.vulkanDevice->commandPool, vulkanInstance.drawCmdBuffers);
    initializers::createCommandBuffers(logicalDevice, vulkanInstance.drawCmdBuffers, vulkanInstance.swapChain, vulkanInstance.vulkanDevice->commandPool);

    buildCommandBuffers(vulkanInstance, ivData, width, height);

    vkDeviceWaitIdle(logicalDevice);

    if ((width > 0.0f) && (height > 0.0f))
    {
        camera.updateAspectRatio((float)width / (float)height);
    }
}

void submitBuffers(VulkanInstance &vulkanInstance, uint32_t& currentBufferIdx)
{
    VK_CHECK_RESULT(vulkanInstance.swapChain.acquireNextImage(vulkanInstance.semaphores.presentComplete, &currentBufferIdx));
    vulkanInstance.submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    vulkanInstance.submitInfo.commandBufferCount = 1;
    vulkanInstance.submitInfo.pCommandBuffers = &vulkanInstance.drawCmdBuffers[currentBufferIdx];
    vulkanInstance.submitInfo.pNext = NULL;
    VK_CHECK_RESULT(vkQueueSubmit(vulkanInstance.queue, 1, &vulkanInstance.submitInfo, VK_NULL_HANDLE));
    VK_CHECK_RESULT(vulkanInstance.swapChain.queuePresent(vulkanInstance.queue, currentBufferIdx, vulkanInstance.semaphores.renderComplete));
    VK_CHECK_RESULT(vkQueueWaitIdle(vulkanInstance.queue));
}

void updateWindowSize(VulkanInstance &vulkanInstance, ImGUI_UI::ImGuiVulkanData& ivData, Camera& camera, int& width, int& height)
{
    static int width_old, height_old;
    // glfwGetWindowSize(vulkanInstance.glfwWindow, &width, &height);
    glfwGetFramebufferSize(vulkanInstance.glfwWindow, &width, &height);
    if (width_old != width || height_old != height)
    {
        ImGui_ImplVulkan_SetMinImageCount(vulkanInstance.swapChain.imageCount);
        ImGui_ImplVulkanH_CreateOrResizeWindow(vulkanInstance.instance, vulkanInstance.vulkanDevice->physicalDevice, 
        vulkanInstance.vulkanDevice->logicalDevice, &vulkanInstance.ImGuiWindow, 
        vulkanInstance.vulkanDevice->queueFamilyIndices.graphics, NULL, width, height, vulkanInstance.swapChain.imageCount);
        vulkanInstance.ImGuiWindow.FrameIndex = 0;
        rebuildBuffers(vulkanInstance, ivData, camera, width, height);
    }
    width_old = width;
    height_old = height;
}

int main()
{

    VulkanInstance vulkanInstance;

    /* Setup, Integration and Initialization of Vulkan/GLFW */

    if (!glfwInit())
        return 1;

    int width = 1280;
    int height = 720;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    vulkanInstance.glfwWindow = glfwCreateWindow(width, height, "Network Viewport", NULL, NULL);

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

    VK_CHECK_RESULT(glfwCreateWindowSurface(vulkanInstance.instance, vulkanInstance.glfwWindow, NULL, &vulkanInstance.surface));

    VulkanDevice *vulkanDevice = vulkanInstance.vulkanDevice;
    setupGLFWVulkanWindow(vulkanInstance, width, height, 2);
    prepareVulkan(vulkanInstance, width, height);

    /* User Interface Settings */

    UISettings uiSettings;
    uiSettings.fontPath = assetPath + "fonts/DroidSansMono.ttf";
    Camera camera;
    camera.type = camera.firstperson;
    camera.position = glm::vec3(0.0f, 0.0f, -10.0f);
    camera.rotation = glm::vec3(-45.0f, 0.0f, 0.0f);
    camera.setPerspective(60.0f, (float)width / (float)height, 0.1f, 1000.0f);

    /* ImGUI App Initialization */

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_Vulkan_Init(vulkanInstance);

    ImGUI_UI::ImGuiVulkanData ivData(vulkanInstance.vulkanDevice);

    ImGUI_UI::setupImGuiVisuals(width, height, uiSettings);

    ImGUI_UI::initializeImGuiVulkanResources(ivData, vulkanInstance.renderPass, vulkanInstance.queue, assetPath + "shaders/");




    /* Render-loop variables */
    bool rebuildSwapChain = false;
    uint32_t currentBufferIdx;
    float frameTimer;
    auto tStart = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(vulkanInstance.glfwWindow))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        auto tEnd = std::chrono::high_resolution_clock::now();
        auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
        frameTimer = (float)tDiff / 1000.0f;

        void newFrame(bool updateFrameGraph, UISettings &uiSettings, float frameTime, Camera& camera);

        ImGUI_UI::newFrame(true, uiSettings, frameTimer, camera);

        ImGUI_UI::updateBuffers(vulkanInstance.vulkanDevice, ivData.vertexBuffer, ivData.indexBuffer, ivData.indexCount, ivData.vertexCount);

        updateWindowSize(vulkanInstance, ivData, camera, width, height);

        buildCommandBuffers(vulkanInstance, ivData, width, height);

        submitBuffers(vulkanInstance, currentBufferIdx);

    }

    ImGui_ImplVulkanH_DestroyWindow(vulkanInstance.instance, vulkanDevice->logicalDevice, &vulkanInstance.ImGuiWindow, NULL);
    vkDestroyDescriptorPool(vulkanDevice->logicalDevice, vulkanInstance.descriptorPool, NULL);

#ifdef IMGUI_VULKAN_DEBUG_REPORT
    // Remove the debug report callback
    auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkDestroyDebugReportCallbackEXT");
    vkDestroyDebugReportCallbackEXT(g_Instance, g_DebugReport, g_Allocator);
#endif // IMGUI_VULKAN_DEBUG_REPORT

    glfwDestroyWindow(vulkanInstance.glfwWindow);
    glfwTerminate();

    return 0;
}