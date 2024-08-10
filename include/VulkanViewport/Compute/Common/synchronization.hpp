#pragma once
#include <VulkanTools/Buffer.hpp>
namespace VkVP::Compute {

void graphicsMemoryBarrierAquire(VulkanBuffer &storageBuffer,
                                 VkCommandBuffer drawCmdBuffer,
                                 uint32_t graphics_QFI, uint32_t compute_QFI);
void graphicsMemoryBarrierRelease(VulkanBuffer &storageBuffer,
                                  VkCommandBuffer drawCmdBuffer,
                                  uint32_t graphics_QFI, uint32_t compute_QFI);
void computeMemoryBarrierAquire(VulkanBuffer &storageBuffer,
                                VkCommandBuffer commandBuffer,
                                uint32_t graphics_QFI, uint32_t compute_QFI);
void computeMemoryBarrierRelease(VulkanBuffer &storageBuffer,
                                 VkCommandBuffer commandBuffer,
                                 uint32_t graphics_QFI, uint32_t compute_QFI);

} // namespace VkVP::Particle