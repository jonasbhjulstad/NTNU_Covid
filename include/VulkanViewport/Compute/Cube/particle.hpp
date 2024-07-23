#pragma once
#include <VulkanTools/Initializers.hpp>
#include <cstdint>
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.h>
#include <tuple>
namespace VkVP::Cube
{
struct Particle {

  glm::vec4 pos;
  glm::vec4 vel;
  glm::vec3 scale;
  glm::mat3 rotation;
  static std::vector<VkVertexInputAttributeDescription>
  getAttributeDescriptions(uint32_t bind_ID, uint32_t layoutOffset) {
    std::vector<VkVertexInputAttributeDescription> attributes(6);
    uint32_t byteOffset = 0;
    attributes[0] = initializers::vertexInputAttributeDescription(
            bind_ID, layoutOffset, VK_FORMAT_R32G32B32A32_SFLOAT, byteOffset);
    layoutOffset ++;
    byteOffset += sizeof(glm::vec4);
    attributes[1] = initializers::vertexInputAttributeDescription(
            bind_ID, layoutOffset, VK_FORMAT_R32G32B32A32_SFLOAT, byteOffset);
    layoutOffset ++;
    byteOffset += sizeof(glm::vec4);
    attributes[2] = initializers::vertexInputAttributeDescription(
            bind_ID, layoutOffset, VK_FORMAT_R32G32B32_SFLOAT, byteOffset);
    layoutOffset ++;
    byteOffset += sizeof(glm::vec3);
    attributes[3] = initializers::vertexInputAttributeDescription(
            bind_ID, layoutOffset, VK_FORMAT_R32G32B32_SFLOAT, byteOffset);
    layoutOffset ++;
    byteOffset += sizeof(glm::vec3);
    attributes[4] = initializers::vertexInputAttributeDescription(
            bind_ID, layoutOffset, VK_FORMAT_R32G32B32_SFLOAT, byteOffset);
    layoutOffset ++;
    byteOffset += sizeof(glm::vec3);
    attributes[5] = initializers::vertexInputAttributeDescription(
            bind_ID, layoutOffset, VK_FORMAT_R32G32B32_SFLOAT, byteOffset);
    layoutOffset ++;
    byteOffset += sizeof(glm::vec3);
    return attributes;
    }
};


std::vector<Particle> createParticles(uint32_t particles_per_attractor);
}