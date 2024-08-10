#pragma once
#include <VulkanTools/InstanceRendering/InstancePipeline.hpp>
#include <VulkanViewport/Compute/Cube/compute.hpp>
#include <VulkanViewport/Compute/Cube/particle.hpp>

namespace VkVP::Cube {
std::unique_ptr<InstancePipelineData> prepareSynchronizedInstancePipeline(
    VulkanInstance &vulkanInstance, VkVP::InstanceRenderingParams &p,
    std::vector<VkVP::Cube::Particle> &instanceData, uint32_t graphics_QFI,
    uint32_t compute_QFI);

}