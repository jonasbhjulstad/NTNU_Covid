#include "compute.hpp"
#include <VulkanTools/GLTF_Assets.hpp>
#include <VulkanTools/Initializers.hpp>
#include <VulkanTools/Tools.hpp>
#include <VulkanViewport/Filepaths.hpp>
#include <glm/glm.hpp>
#include <random>
void buildComputeCommandBuffer(Compute &compute, VulkanBuffer &storageBuffer,
                               uint32_t N_particles) {
  VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();

  VK_CHECK_RESULT(vkBeginCommandBuffer(compute.commandBuffer, &cmdBufInfo));

  // First pass: Calculate particle movement
  // -------------------------------------------------------------------------------------------------------
  vkCmdBindPipeline(compute.commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                    compute.pipelineCalculate);
  vkCmdBindDescriptorSets(compute.commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                          compute.pipelineLayout, 0, 1, &compute.descriptorSet,
                          0, 0);
  vkCmdDispatch(compute.commandBuffer, N_particles / 256, 1, 1);

  // Add memory barrier to ensure that the computer shader has finished writing
  // to the buffer
  VkBufferMemoryBarrier bufferBarrier = initializers::bufferMemoryBarrier();
  bufferBarrier.buffer = storageBuffer.buffer;
  bufferBarrier.size = storageBuffer.descriptor.range;
  bufferBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
  bufferBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  // Transfer ownership if compute and graphics queue family indices differ
  bufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  bufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

  vkCmdPipelineBarrier(compute.commandBuffer,
                       VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                       VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_FLAGS_NONE, 0,
                       nullptr, 1, &bufferBarrier, 0, nullptr);

  // Second pass: Integrate particles
  // -------------------------------------------------------------------------------------------------------
  vkCmdBindPipeline(compute.commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                    compute.pipelineIntegrate);
  vkCmdDispatch(compute.commandBuffer, N_particles / 256, 1, 1);

  //   // Release barrier
  //   if (graphics.queueFamilyIndex != compute.queueFamilyIndex) {
  //     VkBufferMemoryBarrier buffer_barrier = {
  //         VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
  //         nullptr,
  //         VK_ACCESS_SHADER_WRITE_BIT,
  //         0,
  //         compute.queueFamilyIndex,
  //         graphics.queueFamilyIndex,
  //         storageBuffer.buffer,
  //         0,
  //         storageBuffer.size};

  //     vkCmdPipelineBarrier(compute.commandBuffer,
  //                          VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
  //                          VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0,
  //                          nullptr, 1, &buffer_barrier, 0, nullptr);
  //   }

  vkEndCommandBuffer(compute.commandBuffer);
}

// Setup and fill the compute shader storage buffers containing the particles
void prepareStorageBuffers(VulkanInstance &vulkanInstance, Compute &compute,
                           std::vector<Particle> &particleBuffer,
                           VulkanBuffer &storageBuffer) {

  uint32_t N_particles = particleBuffer.size();
  auto &vulkanDevice = vulkanInstance.vulkanDevice;

  compute.uniformData.particleCount = N_particles;

  VkDeviceSize storageBufferSize = particleBuffer.size() * sizeof(Particle);

  // Staging
  // SSBO won't be changed on the host after upload so copy to device local
  // memory

  VulkanBuffer stagingBuffer;

  vulkanDevice->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                             &stagingBuffer, storageBufferSize,
                             particleBuffer.data());
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
  vulkanDevice->flushCommandBuffer(copyCmd, compute.queue, true);

  stagingBuffer.destroy();
}

void updateComputeUniformBuffers(Compute &compute, float frameTimer,
                                 bool paused) {
  compute.uniformData.deltaT = paused ? 0.0f : frameTimer * 0.05f;
  memcpy(compute.uniformBuffer.mapped, &compute.uniformData,
         sizeof(Compute::UniformData));
}

void prepareCompute(VulkanInstance &vulkanInstance, Compute &compute,
                    VulkanBuffer &storageBuffer,
                    VkDescriptorPool& descriptorPool) {
  auto &vulkanDevice = vulkanInstance.vulkanDevice;
  VkDevice device = vulkanDevice->logicalDevice;
  // Create a compute capable device queue
  // The VulkanDevice::createLogicalDevice functions finds a compute capable
  // queue and prefers queue families that only support compute Depending on the
  // implementation this may result in different queue family indices for
  // graphics and computes, requiring proper synchronization (see the memory
  // barriers in buildComputeCommandBuffer)
  vkGetDeviceQueue(device, compute.queueFamilyIndex, 0, &compute.queue);

  // Compute shader uniform buffer block
  vulkanDevice->createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                             &compute.uniformBuffer,
                             sizeof(Compute::UniformData));
  VK_CHECK_RESULT(compute.uniformBuffer.map());

  // Create compute pipeline
  // Compute pipelines are created separate from graphics pipelines even if they
  // use the same queue (family index)

  std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {
      // Binding 0 : Particle position storage buffer
      initializers::descriptorSetLayoutBinding(
          VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 0),
      // Binding 1 : Uniform buffer
      initializers::descriptorSetLayoutBinding(
          VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1),
  };

  VkDescriptorSetLayoutCreateInfo descriptorLayout =
      initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
  VK_CHECK_RESULT(vkCreateDescriptorSetLayout(
      device, &descriptorLayout, nullptr, &compute.descriptorSetLayout));

  VkDescriptorSetAllocateInfo allocInfo =
      initializers::descriptorSetAllocateInfo(descriptorPool,
                                              &compute.descriptorSetLayout, 1);
  VK_CHECK_RESULT(
      vkAllocateDescriptorSets(device, &allocInfo, &compute.descriptorSet));

  std::vector<VkWriteDescriptorSet> computeWriteDescriptorSets = {
      // Binding 0 : Particle position storage buffer
      initializers::writeDescriptorSet(compute.descriptorSet,
                                       VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 0,
                                       &storageBuffer.descriptor),
      // Binding 1 : Uniform buffer
      initializers::writeDescriptorSet(compute.descriptorSet,
                                       VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
                                       &compute.uniformBuffer.descriptor)};
  vkUpdateDescriptorSets(
      device, static_cast<uint32_t>(computeWriteDescriptorSets.size()),
      computeWriteDescriptorSets.data(), 0, nullptr);

  // Create pipelines
  VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo =
      initializers::pipelineLayoutCreateInfo(&compute.descriptorSetLayout, 1);
  VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo,
                                         nullptr, &compute.pipelineLayout));

  VkComputePipelineCreateInfo computePipelineCreateInfo =
      initializers::computePipelineCreateInfo(compute.pipelineLayout, 0);

  // 1st pass
  computePipelineCreateInfo.stage = loadShader(
      device, VkVP::SHADER_DIR + "/ComputeCube/particle_calculate.comp.spv",
      VK_SHADER_STAGE_COMPUTE_BIT);

  // We want to use as much shared memory for the compute shader invocations as
  // available, so we calculate it based on the device limits and pass it to the
  // shader via specialization constants
  uint32_t sharedDataSize = std::min(
      (uint32_t)1024,
      (uint32_t)(vulkanDevice->properties.limits.maxComputeSharedMemorySize /
                 sizeof(glm::vec4)));
  VkSpecializationMapEntry specializationMapEntry =
      initializers::specializationMapEntry(0, 0, sizeof(uint32_t));
  VkSpecializationInfo specializationInfo = initializers::specializationInfo(
      1, &specializationMapEntry, sizeof(int32_t), &sharedDataSize);
  computePipelineCreateInfo.stage.pSpecializationInfo = &specializationInfo;

  VK_CHECK_RESULT(vkCreateComputePipelines(
      device, vulkanInstance.pipelineCache, 1, &computePipelineCreateInfo,
      nullptr, &compute.pipelineCalculate));

  // 2nd pass
  computePipelineCreateInfo.stage = loadShader(
      device, VkVP::SHADER_DIR + "/ComputeCube/particle_integrate.comp.spv",
      VK_SHADER_STAGE_COMPUTE_BIT);
  VK_CHECK_RESULT(vkCreateComputePipelines(
      device, vulkanInstance.pipelineCache, 1, &computePipelineCreateInfo,
      nullptr, &compute.pipelineIntegrate));

  // Separate command pool as queue family for compute may be different than
  // graphics
  VkCommandPoolCreateInfo cmdPoolInfo = {};
  cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  cmdPoolInfo.queueFamilyIndex = compute.queueFamilyIndex;
  cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  VK_CHECK_RESULT(
      vkCreateCommandPool(device, &cmdPoolInfo, nullptr, &compute.commandPool));

  // Create a command buffer for compute operations
  compute.commandBuffer = vulkanDevice->createCommandBuffer(
      VK_COMMAND_BUFFER_LEVEL_PRIMARY, compute.commandPool);

  // Semaphore for compute & graphics sync
  VkSemaphoreCreateInfo semaphoreCreateInfo =
      initializers::semaphoreCreateInfo();
  VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr,
                                    &compute.semaphore));

  // Build a single command buffer containing the compute dispatch commands
}
