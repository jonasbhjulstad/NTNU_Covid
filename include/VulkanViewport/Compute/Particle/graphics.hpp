#pragma once
#include "particle.hpp"
#include <VulkanTools/Buffer.hpp>
#include <VulkanTools/Instance.hpp>
#include <VulkanViewport/UI/ImGuiUI.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.h>
namespace VkVP::Particle {
struct Graphics {
  VkDescriptorSetLayout
      descriptorSetLayout; // Particle system rendering shader binding layout
  VkDescriptorSet descriptorSet;   // Particle system rendering shader bindings
  VkPipelineLayout pipelineLayout; // Layout of the graphics pipeline
  VkPipeline pipeline;             // Particle rendering pipeline
  VkSemaphore
      semaphore; // Execution dependency between compute & graphic submission
  struct UniformData {
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec2 screenDim;
  } uniformData;
  VulkanBuffer uniformBuffer; // Contains scene matrices
};
void createDescriptorPool(VkDevice &device, VkDescriptorPool &descriptorPool);
void buildGraphicsCommandBuffer(VulkanInstance &vulkanInstance,
                                Graphics &graphics,
                                VkCommandBuffer &drawCmdBuffer,
                                VulkanBuffer &storageBuffer,
                                uint32_t N_particles, int width, int height,
                                uint32_t compute_QFI);
void prepareGraphics(VulkanInstance &vulkanInstance, Graphics &graphics,
                     VkDescriptorPool &descriptorPool,
                     ParticleTextures &textures);
void updateGraphicsUniformBuffers(Graphics &graphics, Camera &camera,
                                  uint32_t width, uint32_t height);
} // namespace VkVP::Particle