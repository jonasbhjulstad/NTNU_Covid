#pragma once
#include <VulkanTools/Initializers.hpp>
#include <cstdint>
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.h>
#include <tuple>
struct Particle {

  glm::vec3 pos;
  glm::vec3 vel;
  glm::vec3 scale;
  glm::mat3 rotation;
  static std::vector<VkVertexInputAttributeDescription>
  getAttributeDescriptions(uint32_t bind_ID, uint32_t layoutOffset) {
    return {
        initializers::vertexInputAttributeDescription(
            bind_ID, layoutOffset, VK_FORMAT_R32G32B32_SFLOAT, 0), // Position
        initializers::vertexInputAttributeDescription(
            bind_ID, layoutOffset, VK_FORMAT_R32G32B32_SFLOAT,
            sizeof(glm::vec3)), // velocity
        initializers::vertexInputAttributeDescription(
            bind_ID, layoutOffset, VK_FORMAT_R32G32B32_SFLOAT,
            sizeof(glm::vec3) * 2), // scale
        initializers::vertexInputAttributeDescription(
            bind_ID, layoutOffset, VK_FORMAT_R32G32B32_SFLOAT,
            sizeof(glm::vec3) * 3), // rotation
    };
  }
};


std::vector<Particle> create_particles(uint32_t particles_per_attractor);
