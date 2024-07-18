#pragma once
#include <VulkanTools/Initializers.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>
namespace VkVP {
struct VoxelInstanceData {
  glm::vec3 pos;
  glm::vec3 scale;
  glm::mat3 rotation;
  // Per-Instance attributes
  static std::vector<VkVertexInputAttributeDescription>
  getAttributeDescriptions(uint32_t bind_ID, uint32_t layoutOffset) {
    return {
        initializers::vertexInputAttributeDescription(
            bind_ID, layoutOffset, VK_FORMAT_R32G32B32_SFLOAT, 0), // Position
        initializers::vertexInputAttributeDescription(
            bind_ID, layoutOffset + 1, VK_FORMAT_R32G32B32_SFLOAT,
            sizeof(glm::vec3)), // scale
    };
  }
};
} // namespace VkVP