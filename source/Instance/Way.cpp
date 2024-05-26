#include <VulkanViewport/Instance/Way.hpp>
#include <fstream>
namespace VkVP {

  std::vector<VkVertexInputAttributeDescription>
  WayInstanceData::getAttributeDescriptions(uint32_t bind_ID, uint32_t layoutOffset) {
    return {
        initializers::vertexInputAttributeDescription(
            bind_ID, layoutOffset, VK_FORMAT_R32G32B32_SFLOAT, 0), // Position
        initializers::vertexInputAttributeDescription(
            bind_ID, layoutOffset + 1, VK_FORMAT_R32G32B32_SFLOAT,
            sizeof(glm::vec2)), // scale
        initializers::vertexInputAttributeDescription(
            bind_ID, layoutOffset + 2, VK_FORMAT_R32_SFLOAT,
            sizeof(glm::vec2) + sizeof(float)), // rotation
    };
  }

  std::vector<WayInstanceData> WayInstanceData::read_csv(const std::string& filename)
  {
    std::ifstream file(filename);
    std::vector<WayInstanceData> data;
    std::string line;
    while (std::getline(file, line))
    {
      WayInstanceData instance;
      std::stringstream ss(line);
      std::string token;
      std::getline(ss, token, ',');
      instance.pos.x = std::stof(token);
      std::getline(ss, token, ',');
      instance.pos.y = std::stof(token);
      std::getline(ss, token, ',');
      instance.scale = std::stof(token);
      std::getline(ss, token, ',');
      instance.rotation = std::stof(token);
      data.push_back(instance);
    }
    return data;
  }

  void WayInstanceData::write_csv(const std::string& filename, const std::vector<WayInstanceData>& data)
  {
    std::ofstream file(filename);
    for (const auto& instance : data)
    {
      file << instance.pos.x << "," << instance.pos.y << "," << instance.scale << "," << instance.rotation << "\n";
    }
  }
} // namespace VkVP