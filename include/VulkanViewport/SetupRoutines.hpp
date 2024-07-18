#ifndef SETUP_ROUTINES_HPP
#define SETUP_ROUTINES_HPP
#include <vector>
#include <vulkan/vulkan.hpp>
#include <VulkanTools/Tools.hpp>
#include <VulkanTools/Instance.hpp>
#include <VulkanTools/GLTF_BasicInstance.hpp>
#include <VulkanViewport/UI/ImGuiUI.hpp>

void beginCommandBuffer(VkCommandBuffer commandBuffer)
{
    VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
    VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &cmdBufInfo));
}
void beginRenderPass(VkRenderPass renderPass, VkCommandBuffer commandBuffer, VkFramebuffer frameBuffer, uint32_t width, uint32_t height)
{
    VkClearValue clearValues[2];
    clearValues[0].color = {{67./255, 74./255, 69./255, 1.f}};
    clearValues[1].depthStencil = {1.0f, 0};

    VkRenderPassBeginInfo renderPassBeginInfo = initializers::renderPassBeginInfo();
    renderPassBeginInfo.framebuffer = frameBuffer;

    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.renderArea.offset.x = 0;
    renderPassBeginInfo.renderArea.offset.y = 0;
    renderPassBeginInfo.renderArea.extent.width = width;
    renderPassBeginInfo.renderArea.extent.height = height;
    renderPassBeginInfo.clearValueCount = 2;
    renderPassBeginInfo.pClearValues = clearValues;

    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport = initializers::viewport((float)width, (float)height, 0.0f, 1.0f);
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor = initializers::rect2D(width, height, 0, 0);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void buildCommandBuffers(std::vector<VkCommandBuffer>& commandBuffers, 
std::vector<VkFramebuffer>& frameBuffers,
VkRenderPass renderPass,
VkVP::ImGuiVulkanData& ivData,
const std::vector<std::unique_ptr<glTFBasicInstance::InstancePipelineData>>& instancePipelines,
int width, int height)
{
    // VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
    for (int32_t i = 0; i < commandBuffers.size(); ++i)
    {

        // VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer[i], &cmdBufInfo));
        beginCommandBuffer(commandBuffers[i]);

        beginRenderPass(renderPass, commandBuffers[i], frameBuffers[i], width, height);
        for (auto& instancePipeline : instancePipelines)
        {
            buildCommandBuffer(*instancePipeline, commandBuffers[i]);
        }
        VkVP::drawFrame(ivData, commandBuffers[i]);


        vkCmdEndRenderPass(commandBuffers[i]);

        VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffers[i]));
    }
}

void rebuildBuffers(VulkanInstance &vulkanInstance, const std::vector<std::unique_ptr<glTFBasicInstance::InstancePipelineData>>& instancePipelines, VkVP::ImGuiVulkanData& ivData, Camera &camera, int width, int height)
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

    buildCommandBuffers(vulkanInstance.drawCmdBuffers, vulkanInstance.frameBuffers, vulkanInstance.renderPass, ivData, instancePipelines, width, height);

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
    vulkanInstance.submitInfo.pNext = nullptr;
    VK_CHECK_RESULT(vkQueueSubmit(vulkanInstance.queue, 1, &vulkanInstance.submitInfo, VK_NULL_HANDLE));
    VK_CHECK_RESULT(vulkanInstance.swapChain.queuePresent(vulkanInstance.queue, currentBufferIdx, vulkanInstance.semaphores.renderComplete));
    VK_CHECK_RESULT(vkQueueWaitIdle(vulkanInstance.queue));
}

void updateWindowSize(VulkanInstance &vulkanInstance, VkVP::ImGuiVulkanData& ivData, Camera& camera, const std::vector<std::unique_ptr<glTFBasicInstance::InstancePipelineData>>& instancePipelines, int& width, int& height)
{
    static int width_old, height_old;
    // glfwGetWindowSize(vulkanInstance.glfwWindow, &width, &height);
    glfwGetFramebufferSize(vulkanInstance.glfwWindow, &width, &height);
    if (width_old != width || height_old != height)
    {
        ImGui_ImplVulkan_SetMinImageCount(vulkanInstance.swapChain.imageCount);
        ImGui_ImplVulkanH_CreateOrResizeWindow(vulkanInstance.instance, vulkanInstance.vulkanDevice->physicalDevice, 
        vulkanInstance.vulkanDevice->logicalDevice, &vulkanInstance.ImGuiWindow, 
        vulkanInstance.vulkanDevice->queueFamilyIndices.graphics, nullptr, width, height, vulkanInstance.swapChain.imageCount);
        vulkanInstance.ImGuiWindow.FrameIndex = 0;
        rebuildBuffers(vulkanInstance, instancePipelines, ivData, camera, width, height);
    }
    width_old = width;
    height_old = height;
}


void updateWindowSize(VulkanInstance &vulkanInstance, VkVP::ImGuiVulkanData& ivData, Camera& camera, int& width, int& height)
{
    static int width_old, height_old;
    // glfwGetWindowSize(vulkanInstance.glfwWindow, &width, &height);
    glfwGetFramebufferSize(vulkanInstance.glfwWindow, &width, &height);
    if (width_old != width || height_old != height)
    {
        ImGui_ImplVulkan_SetMinImageCount(vulkanInstance.swapChain.imageCount);
        ImGui_ImplVulkanH_CreateOrResizeWindow(vulkanInstance.instance, vulkanInstance.vulkanDevice->physicalDevice, 
        vulkanInstance.vulkanDevice->logicalDevice, &vulkanInstance.ImGuiWindow, 
        vulkanInstance.vulkanDevice->queueFamilyIndices.graphics, nullptr, width, height, vulkanInstance.swapChain.imageCount);
        vulkanInstance.ImGuiWindow.FrameIndex = 0;
        
    }
    width_old = width;
    height_old = height;
}


#endif