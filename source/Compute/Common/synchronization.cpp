#include <VulkanViewport/Compute/Common/synchronization.hpp>

namespace VkVP::Compute {
void graphicsMemoryBarrierAquire(VulkanBuffer &storageBuffer,
                              VkCommandBuffer drawCmdBuffer,
                              uint32_t graphics_QFI, uint32_t compute_QFI) {
  if (graphics_QFI != compute_QFI) {
    VkBufferMemoryBarrier buffer_barrier = {
        VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
        nullptr,
        0,
        VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
        compute_QFI,
        graphics_QFI,
        storageBuffer.buffer,
        0,
        storageBuffer.size};

    vkCmdPipelineBarrier(drawCmdBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                         VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, 0, 0, nullptr, 1,
                         &buffer_barrier, 0, nullptr);
  }
}
void graphicsMemoryBarrierRelease(VulkanBuffer &storageBuffer,
                                 VkCommandBuffer drawCmdBuffer,
                                 uint32_t graphics_QFI, uint32_t compute_QFI) {
  if (graphics_QFI != compute_QFI) {
    VkBufferMemoryBarrier buffer_barrier = {
        VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
        nullptr,
        VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
        0,
        graphics_QFI,
        compute_QFI,
        storageBuffer.buffer,
        0,
        storageBuffer.size};

    vkCmdPipelineBarrier(drawCmdBuffer, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 1,
                         &buffer_barrier, 0, nullptr);
  }
}

void computeMemoryBarrierAquire(VulkanBuffer &storageBuffer,
                                VkCommandBuffer commandBuffer,
                                uint32_t graphics_QFI, uint32_t compute_QFI) {
  if (graphics_QFI != compute_QFI) {
    VkBufferMemoryBarrier buffer_barrier = {
        VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
        nullptr,
        0,
        VK_ACCESS_SHADER_WRITE_BIT,
        graphics_QFI,
        compute_QFI,
        storageBuffer.buffer,
        0,
        storageBuffer.size};

    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                         VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 0, nullptr, 1,
                         &buffer_barrier, 0, nullptr);
  }
}
    // VkBufferMemoryBarrier buffer_barrier = {
    //     VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
    //     nullptr,
    //     VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
    //     0,
    //     graphics_QFI,
    //     compute.queueFamilyIndex,
    //     storageBuffer.buffer,
    //     0,
    //     storageBuffer.size};

    // vkCmdPipelineBarrier(copyCmd, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
    //                      VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 1,
    //                      &buffer_barrier, 0, nullptr);

void computeMemoryBarrierRelease(VulkanBuffer &storageBuffer,
                                 VkCommandBuffer commandBuffer,
                                 uint32_t graphics_QFI, uint32_t compute_QFI) {
  if (compute_QFI != graphics_QFI) {
    VkBufferMemoryBarrier buffer_barrier = {
        VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
        nullptr,
        VK_ACCESS_SHADER_WRITE_BIT,
        0,
        compute_QFI,
        graphics_QFI,
        storageBuffer.buffer,
        0,
        storageBuffer.size};

    vkCmdPipelineBarrier(commandBuffer,
                         VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 1,
                         &buffer_barrier, 0, nullptr);
  }
}
} // namespace VkVP::Particle