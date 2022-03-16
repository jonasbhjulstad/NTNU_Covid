#ifndef NV_APPLICATION_HPP
#define NV_APPLICATION_HPP
#include <array>
#include "NV_SwapChain.hpp"
#include "NV_GraphicsPipeline.hpp"
#include "NV_Command.hpp"
#include "NV_Synchronization.hpp"
#include "NV_Buffers.hpp"
struct UniformBufferObject
{
    alignas(16) glm::vec2 translation;
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

struct NV_Data
{
    VkDevice logicalDevice;
    VKQueue graphicsQueue, presentQueue;
    VkDescriptorSetLayout descriptorSetLayout;
    PipelineData pipelineData;
    VkCommandPool commandPool;
    std::array<BufferData, 2> vertexBuffers;
    std::array<BufferData, 2> indexBuffers;
    std::vector<VkFrameBuffer> swapChainFrameBuffers;
    std::vector<BufferData> uniformBuffers;
    VkDescriptorPool descriptorPool;
    std::vector<VkCommandBuffer> commandBuffers;
    FrameSemaphores frameSemaphores;
}





createCommandBuffers(logicalDevice);
FrameSemaphores frameSemaphores = createFrameSemaphores(logicalDevice);

#endif
