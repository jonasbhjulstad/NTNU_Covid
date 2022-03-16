#ifndef NV_APPLICATION_HPP
#define NV_APPLICATION_HPP
#include <array>
#include "NV_SwapChain.hpp"
#include "NV_Queue.hpp"
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
    VkQueue graphicsQueue, presentQueue;
    VkDescriptorSetLayout descriptorSetLayout;
    PipelineData pipelineData;
    VkCommandPool commandPool;
    std::array<Buffer, 2> vertexBuffers;
    std::array<Buffer, 2> indexBuffers;
    std::vector<VkFramebuffer> swapChainFrameBuffers;
    std::vector<Buffer> uniformBuffers;
    VkDescriptorPool descriptorPool;
    std::vector<VkCommandBuffer> commandBuffers;
    FrameSemaphores frameSemaphores;
};
namespace NV
{
    const std::vector<Vertex> createCircleVertices(const float &radius, const uint32_t N_vertices, const glm::vec3 color = {1.0f, .0f, .0f});
    const std::vector<uint16_t> createCircleIndices(const uint32_t N_vertices);

    NV_Data createNetworkViewport(VkSurfaceKHR surface,
                                  GLFWwindow *window,
                                  VkPhysicalDevice physicalDevice);
    void cleanupNetworkViewport(NV_Data &data, const size_t maxFrames = 2);
}

#endif
