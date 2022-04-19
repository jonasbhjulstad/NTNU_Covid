#ifndef NV_BEZIER_HPP
#define NV_BEZIER_HPP
#include "vulkanexamplebase.h"
#include <glm/glm.hpp>
#include <vector>
struct BezierInstanceData
{
    glm::vec3 startNode;
    glm::vec3 endNode;
    glm::vec3 scale;

    // Per-Instance attributes
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(uint32_t bind_ID, uint32_t layoutOffset)
    {
        return {
            vks::initializers::vertexInputAttributeDescription(bind_ID, layoutOffset, VK_FORMAT_R32G32B32_SFLOAT, 0),                     // StartPos
            vks::initializers::vertexInputAttributeDescription(bind_ID, layoutOffset + 1, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float)*3),                     // EndPos
            vks::initializers::vertexInputAttributeDescription(bind_ID, layoutOffset + 2, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float)*6),                     // Scale
        };
    }
};

#endif