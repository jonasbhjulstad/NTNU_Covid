#include "NV_Application.hpp"
namespace NV
{
    const std::vector<Vertex> createCircleVertices(const float &radius, const uint32_t N_vertices, const glm::vec3 color = {1.0f, .0f, .0f})
    {
        std::vector<Vertex> vertices(N_vertices + 1);
        vertices[0] = {{.0f, .0f}, color};
        for (uint32_t i = 0; i < N_vertices; i++)
        {
            vertices[i] = {{radius * cos(2.0f * M_PI / (float)N_vertices * (float)i), radius * sin(2.0f * M_PI / (float)N_vertices * (float)i)}, color};
        }
        return vertices;
    }

    static const uint16_t N_vertices = 10;
    const std::vector<Vertex> vertices = createCircleVertices(1.0, N_vertices);

    const std::vector<uint16_t> createCircleIndices(const uint32_t N_vertices)
    {
        std::vector<uint16_t> indices(3 * N_vertices);
        for (uint32_t i = 0; i < N_vertices; i++)
        {
            indices[3 * i] = 0;
            indices[3 * i + 1] = 1 + i;
            indices[3 * i + 2] = 2 + i;
        }
        return indices;
    }

    static const std::vector<uint16_t> indices = createCircleIndices(N_vertices);

    NV_Data createNetworkViewport(VkSurfaceKHR surface,
                                  GLFWwindow *window,
                                  VkPhysicalDevice physicalDevice)
    {
        NV_Data data;
        createLogicalDevice(physicalDevice, data.graphicsQueue, data.presentQueue, data.logicalDevice);

        data.descriptorSetLayout = createDescriptorSetLayout(data.logicalDevice);

        data.pipelineData.swapChainData = createSwapChain(surface, window, data.logicalDevice, physicalDevice);

        std::string shaderPath = "/home/deb/Documents/VulkanTutorial/build/shaders/";
        data.pipelineData = createGraphicsPipeline(shaderPath + "shader.vert",
                                                   shaderPath + "shader.frag",
                                                   data.swapChainData.extent,
                                                   data.descriptorSetLayout,
                                                   data.logicalDevice);

        data.commandPool = createCommandPool(physicalDevice);

        data.vertexBuffers = createStagedVertexBuffer(vertices, data.commandPool, data.graphicsQueue, data.logicalDevice);
        data.indexBuffers = createStagedIndexBuffer(indices, data.commandPool, data.graphicsQueue, data.logicalDevice);

        size_t N_buffers = swapChainData.imageViews.size();
        data.swapChainFrameBuffers = createFramebuffers(data.swapChainData, data.pipelineData.renderPass, data.logicalDevice);

        data.uniformBuffers = createUniformBuffers(sizeof(UniformBufferObject), data.logicalDevice, physicalDevice, 0);

        data.descriptorPool = createDescriptorPool(data.logicalDevice);
        createDescriptorSets(data.descriptorSetLayout, data.descriptorPool, data.logicalDevice);

        data.commandBuffers = createCommandBuffers(data.commandPool, data.logicalDevice);
        data.frameSemaphores = createFrameSemaphores(data.logicalDevice);

        return data;
    }

    void cleanupNetworkViewport(NV_Data &data, const size_t maxFrames = 2)
    {
        cleanupSwapChain(data.swapChainData,
                         data.swapChainFrameBuffers,
                         data.pipeline,
                         data.pipelineLayout,
                         data.pipeline.renderPass,
                         data.logicalDevice);

        for (size_t i = 0; i < maxFrames; i++)
        {
            vkDestroyBuffer(data.logicalDevice, uniformBuffers[i], nullptr);
            vkFreeMemory(data.logicalDevice, uniformBuffersMemory[i], nullptr);
        }

        vkDestroyDescriptorPool(data.logicalDevice, data.descriptorPool, nullptr);

        vkDestroyDescriptorSetLayout(data.logicalDevice, data.descriptorSetLayout, nullptr);

        vkDestroyBuffer(data.logicalDevice, data.indexBuffer, nullptr);
        vkFreeMemory(data.logicalDevice, data.indexBufferMemory, nullptr);

        vkDestroyBuffer(data.logicalDevice, data.vertexBuffer, nullptr);
        vkFreeMemory(data.logicalDevice, data.vertexBufferMemory, nullptr);

        for (size_t i = 0; i < maxFrames; i++)
        {
            vkDestroySemaphore(data.logicalDevice, data.frameSemaphores.renderFinished[i], nullptr);
            vkDestroySemaphore(data.logicalDevice, data.frameSemaphores.imageAvailable[i], nullptr);
            vkDestroyFence(data.logicalDevice, data.frameSemaphores.flight[i], nullptr);
        }
        vkDestroyCommandPool(data.logicalDevice, data.commandPool, nullptr);

        vkDestroyDevice(data.logicalDevice, nullptr);

        // if (enableValidationLayers)
        // {
        //     DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        // }

    }

}