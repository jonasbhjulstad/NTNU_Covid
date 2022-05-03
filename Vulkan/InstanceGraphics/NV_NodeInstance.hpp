#ifndef NV_NODE_INSTANCE_HPP
#define NV_NODE_INSTANCE_HPP
#include <vector>
#include <vulkan/vulkan.hpp>
#include <igraph/igraph.h>
struct NodeInstanceData
{
    glm::vec3 pos;
    glm::vec4 color;
    float scale;
    // Per-Instance attributes
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(uint32_t bind_ID, uint32_t layoutOffset)
    {
        return {
            initializers::vertexInputAttributeDescription(bind_ID, layoutOffset, VK_FORMAT_R32G32B32_SFLOAT, 0),// Position
            initializers::vertexInputAttributeDescription(bind_ID, layoutOffset + 1, VK_FORMAT_R32_SFLOAT, sizeof(float)*3),// scale
        };
    }

};


#endif
