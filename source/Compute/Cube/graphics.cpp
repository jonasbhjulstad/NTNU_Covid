#include <VulkanViewport/Compute/Cube/graphics.hpp>
#include <memory>
namespace VkVP::Cube
{


static void prepareStorageBuffers(VulkanInstance &vulkanInstance,
                           std::vector<Particle> &particles,
                           VulkanBuffer &storageBuffer, uint32_t graphics_QFI, uint32_t compute_QFI) {

  uint32_t N_particles = particles.size();
  auto &vulkanDevice = vulkanInstance.vulkanDevice;

  VkDeviceSize storageBufferSize = particles.size() * sizeof(Particle);

  // Staging
  // SSBO won't be changed on the host after upload so copy to device local
  // memory

  VulkanBuffer stagingBuffer;

  vulkanDevice->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                             &stagingBuffer, storageBufferSize,
                             particles.data());
  // The SSBO will be used as a storage buffer for the compute pipeline and as a
  // vertex buffer in the graphics pipeline
  vulkanDevice->createBuffer(
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
          VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &storageBuffer, storageBufferSize);

  // Copy from staging buffer to storage buffer
  VkCommandBuffer copyCmd =
      vulkanDevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
  VkBufferCopy copyRegion = {};
  copyRegion.size = storageBufferSize;
  vkCmdCopyBuffer(copyCmd, stagingBuffer.buffer, storageBuffer.buffer, 1,
                  &copyRegion);

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

    vkCmdPipelineBarrier(copyCmd, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 1,
                         &buffer_barrier, 0, nullptr);
  }
  vulkanDevice->flushCommandBuffer(copyCmd, vulkanInstance.queue, true);

  stagingBuffer.destroy();
}

std::unique_ptr<InstancePipelineData> prepareSynchronizedInstancePipeline(VulkanInstance& vulkanInstance, VkVP::InstanceRenderingParams& p, std::vector<VkVP::Cube::Particle>& instanceData, uint32_t graphics_QFI, uint32_t compute_QFI)
{
    using namespace VkVP;
    VulkanDevice* vulkanDevice = p.vulkanDevice;
    VkDevice device = vulkanDevice->logicalDevice;
    std::unique_ptr<InstancePipelineData> instancePipelineData = std::make_unique<InstancePipelineData>(device);

    prepareStorageBuffers(vulkanInstance, instanceData, instancePipelineData->instanceBuffer, graphics_QFI, compute_QFI);
    instancePipelineData->model = loadModel(p.modelPath, p.vulkanDevice, p.queue);
    if (!p.texturePath.empty())
    {
        instancePipelineData->texture = loadTexture(p.texturePath, p.vulkanDevice, p.queue);
        instancePipelineData->descriptorSetLayout = textureDescriptorSetLayout(device);
    }
    else
    {
        instancePipelineData->descriptorSetLayout = uniformDescriptorSetLayout(device);
    }
    instancePipelineData->pipelineLayout = setupPipelineLayout(device, instancePipelineData->descriptorSetLayout);
    instancePipelineData->descriptorSet = setupDescriptorSets(device, instancePipelineData->descriptorSetLayout, p.descriptorPool, p.uniformProjectionBuffer, nullptr);
    instancePipelineData->pipeline = setupPipeline<VkVP::Cube::Particle>(p.vertexShaderPath, p.fragmentShaderPath, instancePipelineData->pipelineLayout, instancePipelineData->shaderStages, device, p.renderPass, p.pipelineCache);
    instancePipelineData->offset = p.offset;
    instancePipelineData->N_instances = instanceData.size();
    return instancePipelineData;
} 
}