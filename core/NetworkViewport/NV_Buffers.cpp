#include "NV_Buffers.hpp"

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void createBuffer(VkDeviceSize size,
                  VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties,
                  VkBuffer &buffer,
                  VkDeviceMemory &bufferMemory,
                  VkDevice logicalDevice,
                  VkPhysicalDevice physicalDevice)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(logicalDevice, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);

    if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(logicalDevice, buffer, bufferMemory, 0);
}

void copyBuffer(VkBuffer srcBuffer,
                VkBuffer dstBuffer,
                VkDeviceSize size,
                VkCommandPool commandPool,
                VkQueue graphicsQueue,
                VkDevice logicalDevice)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
}

std::vector<Buffer> createUniformBuffers(VkDeviceSize bufferSize,
                                             VkDevice logicalDevice,
                                             VkPhysicalDevice physicalDevice,
                                             VkMemoryPropertyFlagBits memFlags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                             const uint32_t maxFrames = 2)
{

    std::vector<Buffer> uniformBuffers(maxFrames);

    for (size_t i = 0; i < maxFrames; i++)
    {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | memFlags, uniformBuffers[i].buffer, uniformBuffers[i].memory, logicalDevice, physicalDevice);
    }
    return uniformBuffers;
}

std::array<Buffer, 2> createStagedVertexBuffer(const std::vector<Vertex> &vertices,
                                               VkCommandPool commandPool,
                                               VkQueue graphicsQueue,
                                               VkDevice logicalDevice)
{
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    Buffer stagingBuffer;
    createBuffer(bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer.buffer,
                 stagingBuffer.memory,
                 logicalDevice);

    void *data;
    vkMapMemory(logicalDevice, stagingBuffer.memory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(logicalDevice, stagingBuffer.memory);

    Buffer gpuBuffer;

    createBuffer(bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 gpuBuffer.buffer,
                 gpuBuffer.memory,
                 logicalDevice);

    copyBuffer(stagingBuffer.buffer,
               gpuBuffer.buffer,
               bufferSize,
               commandPool,
               graphicsQueue,
               logicalDevice);

    vkDestroyBuffer(logicalDevice, stagingBuffer.buffer, nullptr);
    vkFreeMemory(logicalDevice, stagingBuffer.memory, nullptr);

    return {stagingBuffer, gpuBuffer};
}

std::array<Buffer, 2> createStagedIndexBuffer(const std::vector<uint32_t> &indices,
                                              VkCommandPool commandPool,
                                              VkQueue graphicsQueue,
                                              VkDevice logicalDevice)
{
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    Buffer stagingBuffer;
    createBuffer(bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer.buffer,
                 stagingBuffer.memory,
                 logicalDevice);

    void *data;
    vkMapMemory(logicalDevice, , 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(logicalDevice, stagingBuffer.memory);

    Buffer gpuBuffer;

    createBuffer(bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 gpuBuffer.buffer,
                 gpuBuffer.memory,
                 logicalDevice);

    copyBuffer(stagingBuffer.buffer, gpuBuffer.buffer, bufferSize, commandPool, graphicsQueue, logicalDevice);

    vkDestroyBuffer(logicalDevice, stagingBuffer.buffer, nullptr);
    vkFreeMemory(logicalDevice, stagingBuffer.memory, nullptr);

    return {stagingBuffer, gpuBuffer};
}

std::vector<VkFramebuffer> createFramebuffers(VkRenderPass renderPass,
                                              std::vector<VkImageView> &swapChainImageViews,
                                              VkExtent2D swapChainExtent,
                                              VkDevice logicalDevice)
{
    const uint32_t N_buffers = swapChainImageViews.size();
    std::vector<VkFramebuffer> frameBuffers(N_buffers);

    for (size_t i = 0; i < N_buffers; i++)
    {
        VkImageView attachments[] = {
            swapChainImageViews[i]};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &frameBuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
    return frameBuffers;
}

std::vector<VkCommandBuffer> createCommandBuffers(VkCommandPool commandPool, VkDevice logicalDevice, const uint32_t maxFrames = 2)
{
    std::vector<VkCommandBuffer> commandBuffers(maxFrames);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    return commandBuffers;
}