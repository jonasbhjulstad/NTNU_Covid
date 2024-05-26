#pragma once
#include <VulkanTools/Initializers.hpp>
#include <glm/glm.hpp>
#include <igraph/igraph.h>
#include <vector>
namespace VkVP {
struct WayInstanceData {
  glm::vec2 pos;
  float scale;
  float rotation;
  // Per-Instance attributes
  static std::vector<VkVertexInputAttributeDescription>
  getAttributeDescriptions(uint32_t bind_ID, uint32_t layoutOffset);
  static std::vector<WayInstanceData>
  read_csv(const std::string &filename);
  static void
  write_csv(const std::string &filename,
                             const std::vector<WayInstanceData> &data);
};
} // namespace VkVP