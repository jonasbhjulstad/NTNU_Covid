#pragma once
#include "particle.hpp"
#include <VulkanTools/Buffer.hpp>
#include <VulkanTools/Instance.hpp>
#include <vulkan/vulkan.h>

struct Compute {
  uint32_t queueFamilyIndex; // Used to check if compute and graphics queue
                             // families differ and require additional barriers
  VkQueue queue; // Separate queue for compute commands (queue family may differ
                 // from the one used for graphics)
  VkCommandPool commandPool; // Use a separate command pool (queue family may
                             // differ from the one used for graphics)
  VkCommandBuffer commandBuffer; // Command buffer storing the dispatch commands
                                 // and barriers
  VkSemaphore
      semaphore; // Execution dependency between compute & graphic submission
  VkDescriptorSetLayout descriptorSetLayout; // Compute shader binding layout
  VkDescriptorSet descriptorSet;             // Compute shader bindings
  VkPipelineLayout pipelineLayout;           // Layout of the compute pipeline
  VkPipeline pipelineCalculate; // Compute pipeline for N-Body velocity
                                // calculation (1st pass)
  VkPipeline
      pipelineIntegrate; // Compute pipeline for euler integration (2nd pass)
  struct UniformData {   // Compute shader uniform block object
    float deltaT{0.0f};  // Frame delta time
    int32_t particleCount{0};
    // Parameters used to control the behaviour of the particle system
    float gravity{0.002f};
    float power{0.75f};
    float soften{0.05f};
  } uniformData;
  VulkanBuffer uniformBuffer; // Uniform buffer object containing particle
                              // system parameters
};
void buildComputeCommandBuffer(Compute &compute, VulkanBuffer &storageBuffer,
                               uint32_t N_particles, uint32_t graphics_QFI);
void prepareStorageBuffers(VulkanInstance &vulkanInstance, Compute &compute,
                           std::vector<Particle> &particleBuffer,
                           VulkanBuffer &storageBuffer, uint32_t graphics_QFI);
void updateComputeUniformBuffers(Compute &compute, float frameTimer,
                                 bool paused = false);
void prepareCompute(VulkanInstance &vulkanInstance, Compute &compute,
                    VulkanBuffer &storageBuffer,
                    VkDescriptorPool &descriptorPool);