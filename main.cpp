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

static void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data, VulkanDevice* vulkanDevice, VkQueue queue, bool& swapChainRebuild)
{
    VkResult err;

    VkSemaphore image_acquired_semaphore  = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
    VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
    err = vkAcquireNextImageKHR(vulkanDevice->logicalDevice, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        swapChainRebuild = true;
        return;
    }
    check_vk_result(err);

    ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
    {
        err = vkWaitForFences(vulkanDevice->logicalDevice, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
        check_vk_result(err);

        err = vkResetFences(vulkanDevice->logicalDevice, 1, &fd->Fence);
        check_vk_result(err);
    }
    {
        err = vkResetCommandPool(vulkanDevice->logicalDevice, fd->CommandPool, 0);
        check_vk_result(err);
        VkCommandBufferBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
        check_vk_result(err);
    }
    {
        VkRenderPassBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.renderPass = wd->RenderPass;
        info.framebuffer = fd->Framebuffer;
        info.renderArea.extent.width = wd->Width;
        info.renderArea.extent.height = wd->Height;
        info.clearValueCount = 1;
        info.pClearValues = &wd->ClearValue;
        vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    // Record dear imgui primitives into command buffer
    ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

    // Submit command buffer
    vkCmdEndRenderPass(fd->CommandBuffer);
    {
        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &image_acquired_semaphore;
        info.pWaitDstStageMask = &wait_stage;
        info.commandBufferCount = 1;
        info.pCommandBuffers = &fd->CommandBuffer;
        info.signalSemaphoreCount = 1;
        info.pSignalSemaphores = &render_complete_semaphore;

        err = vkEndCommandBuffer(fd->CommandBuffer);
        check_vk_result(err);
        err = vkQueueSubmit(queue, 1, &info, fd->Fence);
        check_vk_result(err);
    }
}

static void FramePresent(ImGui_ImplVulkanH_Window* wd, VkQueue queue, bool& rebuildSwapChain)
{
    if (rebuildSwapChain)
        return;
    VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &render_complete_semaphore;
    info.swapchainCount = 1;
    info.pSwapchains = &wd->Swapchain;
    info.pImageIndices = &wd->FrameIndex;
    VkResult err = vkQueuePresentKHR(queue, &info);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        rebuildSwapChain = true;
        return;
    }
    check_vk_result(err);
    wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount; // Now we can use the next set of semaphores
}

int main()
{
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


        // Upload Fonts
    {
        // Use any command queue
        VkCommandPool command_pool = vulkanInstance.ImGuiWindow.Frames[vulkanInstance.ImGuiWindow.FrameIndex].CommandPool;
        VkCommandBuffer command_buffer = vulkanInstance.ImGuiWindow.Frames[vulkanInstance.ImGuiWindow.FrameIndex].CommandBuffer;

        // VK_CHECK_RESULT(vkResetCommandPool(vulkanDevice->logicalDevice, command_pool, 0));
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        VK_CHECK_RESULT(vkBeginCommandBuffer(command_buffer, &begin_info));

        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

        VkSubmitInfo end_info = {};
        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        end_info.commandBufferCount = 1;
        end_info.pCommandBuffers = &command_buffer;
        VK_CHECK_RESULT(vkEndCommandBuffer(command_buffer));
        VK_CHECK_RESULT(vkQueueSubmit(vulkanInstance.queue, 1, &end_info, VK_NULL_HANDLE));

        VK_CHECK_RESULT(vkDeviceWaitIdle(vulkanDevice->logicalDevice));
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }
    bool rebuildSwapChain = true;
        // Main loop
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(vulkanInstance.glfwWindow))
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
                ImGui_ImplVulkanH_CreateOrResizeWindow(vulkanInstance.instance, vulkanDevice->physicalDevice, vulkanDevice->logicalDevice, &vulkanInstance.ImGuiWindow, vulkanDevice->queueFamilyIndices.graphics, NULL, width, height, vulkanInstance.swapChain.imageCount);
                vulkanInstance.ImGuiWindow.FrameIndex = 0;
                rebuildSwapChain = false;
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bool demoWindowOpen = true;
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        ImGui::ShowDemoWindow(&demoWindowOpen);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }



        // Rendering
        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();
        const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
        if (!is_minimized)
        {
            vulkanInstance.ImGuiWindow.ClearValue.color.float32[0] = clear_color.x * clear_color.w;
            vulkanInstance.ImGuiWindow.ClearValue.color.float32[1] = clear_color.y * clear_color.w;
            vulkanInstance.ImGuiWindow.ClearValue.color.float32[2] = clear_color.z * clear_color.w;
            vulkanInstance.ImGuiWindow.ClearValue.color.float32[3] = clear_color.w;

            FrameRender(&vulkanInstance.ImGuiWindow, draw_data, vulkanDevice, vulkanInstance.queue, rebuildSwapChain);
            FramePresent(&vulkanInstance.ImGuiWindow, vulkanInstance.queue, rebuildSwapChain);
        }
    }

    // Cleanup
    VK_CHECK_RESULT(vkDeviceWaitIdle(vulkanDevice->logicalDevice));
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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