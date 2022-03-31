#ifndef NV_NODE_HPP
#define NV_NODE_HPP
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
struct NodeInstanceData
{
    glm::vec3 pos;
    float scale;

    // Per-Instance attributes
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(uint32_t bind_ID, uint32_t layoutOffset)
    {
        return {
            vks::initializers::vertexInputAttributeDescription(bind_ID, layoutOffset, VK_FORMAT_R32G32B32_SFLOAT, 0),// Position
            vks::initializers::vertexInputAttributeDescription(bind_ID, layoutOffset + 1, VK_FORMAT_R32_SFLOAT, sizeof(float)*3),// Position
        };
    }
};

#endif