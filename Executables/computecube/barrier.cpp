#include "barrier.hpp"
void graphics_compute_barrier_release(uint32_t graphics_QFI, uint32_t compute_QFI) {
  if (graphics_QFI != compute_QFI) {
    VkBufferMemoryBarrier buffer_barrier = {
        VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
        nullptr,
        VK_ACCESS_SHADER_WRITE_BIT,
        0,
        compute.queueFamilyIndex,
        graphics.queueFamilyIndex,
        storageBuffer.buffer,
        0,
        storageBuffer.size};

    vkCmdPipelineBarrier(compute.commandBuffer,
                         VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 1,
                         &buffer_barrier, 0, nullptr);
  }
}