#include <VulkanViewport/Compute/Particle/graphics.hpp>
#include <VulkanTools/GLTF_Assets.hpp>
#include <VulkanTools/Initializers.hpp>
#include <VulkanViewport/Filepaths.hpp>
namespace VkVP::Particle {
void buildGraphicsCommandBuffer(VulkanInstance &vulkanInstance, Graphics &graphics,
                         VkCommandBuffer &drawCmdBuffer,
                         VulkanBuffer &storageBuffer, uint32_t N_particles,
                         int width, int height, uint32_t compute_QFI) {
  auto &vulkanDevice = vulkanInstance.vulkanDevice;
  auto &device = vulkanInstance.vulkanDevice->logicalDevice;
  auto &frameBuffers = vulkanInstance.frameBuffers;
  VkRenderPass &renderPass = vulkanInstance.renderPass;


  VkViewport viewport =
      initializers::viewport((float)width, (float)height, 0.0f, 1.0f);
  vkCmdSetViewport(drawCmdBuffer, 0, 1, &viewport);

  VkRect2D scissor = initializers::rect2D(width, height, 0, 0);
  vkCmdSetScissor(drawCmdBuffer, 0, 1, &scissor);

  vkCmdBindPipeline(drawCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    graphics.pipeline);
  vkCmdBindDescriptorSets(drawCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          graphics.pipelineLayout, 0, 1,
                          &graphics.descriptorSet, 0, nullptr);

  VkDeviceSize offsets[1] = {0};
  vkCmdBindVertexBuffers(drawCmdBuffer, 0, 1, &storageBuffer.buffer,
                         offsets);
  vkCmdDraw(drawCmdBuffer, N_particles, 1, 0, 0);

}

void createDescriptorPool(VkDevice &device, VkDescriptorPool &descriptorPool) {
  std::vector<VkDescriptorPoolSize> poolSizes = {
      initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2),
      initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1),
      initializers::descriptorPoolSize(
          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2)};
  VkDescriptorPoolCreateInfo descriptorPoolInfo =
      initializers::descriptorPoolCreateInfo(poolSizes, 2);
  VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr,
                                         &descriptorPool));
}

void prepareGraphics(VulkanInstance &vulkanInstance, Graphics &graphics,
                     VkDescriptorPool &descriptorPool, ParticleTextures& textures) {
  auto &vulkanDevice = vulkanInstance.vulkanDevice;
  auto &device = vulkanDevice->logicalDevice;
  VkQueue queue = vulkanInstance.queue;
  // Vertex shader uniform buffer block
  vulkanDevice->createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                             &graphics.uniformBuffer,
                             sizeof(Graphics::UniformData));
  VK_CHECK_RESULT(graphics.uniformBuffer.map());

  // Descriptor pool

  // Descriptor layout
  std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
  setLayoutBindings = {
      initializers::descriptorSetLayoutBinding(
          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
          VK_SHADER_STAGE_FRAGMENT_BIT, 0),
      initializers::descriptorSetLayoutBinding(
          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
          VK_SHADER_STAGE_FRAGMENT_BIT, 1),
      initializers::descriptorSetLayoutBinding(
          VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 2),
  };

  VkDescriptorSetLayoutCreateInfo descriptorLayout =
      initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
  VK_CHECK_RESULT(vkCreateDescriptorSetLayout(
      device, &descriptorLayout, nullptr, &graphics.descriptorSetLayout));

  // Descriptor set
  VkDescriptorSetAllocateInfo allocInfo =
      initializers::descriptorSetAllocateInfo(descriptorPool,
                                              &graphics.descriptorSetLayout, 1);
  VK_CHECK_RESULT(
      vkAllocateDescriptorSets(device, &allocInfo, &graphics.descriptorSet));

  std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
      initializers::writeDescriptorSet(
          graphics.descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0,
          &textures.particle.descriptor),
      initializers::writeDescriptorSet(
          graphics.descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1,
          &textures.gradient.descriptor),
      initializers::writeDescriptorSet(graphics.descriptorSet,
                                       VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2,
                                       &graphics.uniformBuffer.descriptor),
  };
  vkUpdateDescriptorSets(device,
                         static_cast<uint32_t>(writeDescriptorSets.size()),
                         writeDescriptorSets.data(), 0, nullptr);

  // Pipeline layout
  VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo =
      initializers::pipelineLayoutCreateInfo(&graphics.descriptorSetLayout, 1);
  VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo,
                                         nullptr, &graphics.pipelineLayout));

  // Pipeline
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyState =
      initializers::pipelineInputAssemblyStateCreateInfo(
          VK_PRIMITIVE_TOPOLOGY_POINT_LIST, 0, VK_FALSE);
  VkPipelineRasterizationStateCreateInfo rasterizationState =
      initializers::pipelineRasterizationStateCreateInfo(
          VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE,
          VK_FRONT_FACE_COUNTER_CLOCKWISE, 0);
  VkPipelineColorBlendAttachmentState blendAttachmentState =
      initializers::pipelineColorBlendAttachmentState(0xf, VK_FALSE);
  VkPipelineColorBlendStateCreateInfo colorBlendState =
      initializers::pipelineColorBlendStateCreateInfo(1, &blendAttachmentState);
  VkPipelineDepthStencilStateCreateInfo depthStencilState =
      initializers::pipelineDepthStencilStateCreateInfo(VK_FALSE, VK_FALSE,
                                                        VK_COMPARE_OP_ALWAYS);
  VkPipelineViewportStateCreateInfo viewportState =
      initializers::pipelineViewportStateCreateInfo(1, 1, 0);
  VkPipelineMultisampleStateCreateInfo multisampleState =
      initializers::pipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT,
                                                       0);
  std::vector<VkDynamicState> dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT,
                                                     VK_DYNAMIC_STATE_SCISSOR};
  VkPipelineDynamicStateCreateInfo dynamicState =
      initializers::pipelineDynamicStateCreateInfo(dynamicStateEnables);
  std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages;

  // Vertex Input state
  std::vector<VkVertexInputBindingDescription> inputBindings = {
      initializers::vertexInputBindingDescription(0, sizeof(Particle),
                                                  VK_VERTEX_INPUT_RATE_VERTEX)};
  std::vector<VkVertexInputAttributeDescription> inputAttributes = {
      // Location 0 : Position
      initializers::vertexInputAttributeDescription(
          0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, 0),
      // Location 1 : Velocity (used for color gradient lookup)
      initializers::vertexInputAttributeDescription(
          0, 1, VK_FORMAT_R32G32B32A32_SFLOAT, sizeof(glm::vec4)),
  };
  VkPipelineVertexInputStateCreateInfo vertexInputState =
      initializers::pipelineVertexInputStateCreateInfo();
  vertexInputState.vertexBindingDescriptionCount =
      static_cast<uint32_t>(inputBindings.size());
  vertexInputState.pVertexBindingDescriptions = inputBindings.data();
  vertexInputState.vertexAttributeDescriptionCount =
      static_cast<uint32_t>(inputAttributes.size());
  vertexInputState.pVertexAttributeDescriptions = inputAttributes.data();
  using namespace VkVP;
  // Shaders
  shaderStages[0] =
      loadShader(device, VkVP::SHADER_DIR + "/ComputeCube/particle.vert.spv",
                 VK_SHADER_STAGE_VERTEX_BIT);
  shaderStages[1] =
      loadShader(device, VkVP::SHADER_DIR + "/ComputeCube/particle.frag.spv",
                 VK_SHADER_STAGE_FRAGMENT_BIT);

  VkGraphicsPipelineCreateInfo pipelineCreateInfo =
      initializers::pipelineCreateInfo(graphics.pipelineLayout,
                                       vulkanInstance.renderPass, 0);
  pipelineCreateInfo.pVertexInputState = &vertexInputState;
  pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
  pipelineCreateInfo.pRasterizationState = &rasterizationState;
  pipelineCreateInfo.pColorBlendState = &colorBlendState;
  pipelineCreateInfo.pMultisampleState = &multisampleState;
  pipelineCreateInfo.pViewportState = &viewportState;
  pipelineCreateInfo.pDepthStencilState = &depthStencilState;
  pipelineCreateInfo.pDynamicState = &dynamicState;
  pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
  pipelineCreateInfo.pStages = shaderStages.data();
  pipelineCreateInfo.renderPass = vulkanInstance.renderPass;

  // Additive blending
  blendAttachmentState.colorWriteMask = 0xF;
  blendAttachmentState.blendEnable = VK_TRUE;
  blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
  blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
  blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
  blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_DST_ALPHA;

  VK_CHECK_RESULT(vkCreateGraphicsPipelines(
      device, vulkanInstance.pipelineCache, 1, &pipelineCreateInfo, nullptr,
      &graphics.pipeline));

  // We use a semaphore to synchronize compute and graphics
  VkSemaphoreCreateInfo semaphoreCreateInfo =
      initializers::semaphoreCreateInfo();
  VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr,
                                    &graphics.semaphore));

  // Signal the semaphore for the first run
  VkSubmitInfo submitInfo = initializers::submitInfo();
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = &graphics.semaphore;
  VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
  VK_CHECK_RESULT(vkQueueWaitIdle(queue));
}
void updateGraphicsUniformBuffers(Graphics &graphics, Camera &camera,
                                  uint32_t width, uint32_t height) {
  graphics.uniformData.projection = camera.matrices.perspective;
  graphics.uniformData.view = camera.matrices.view;
  graphics.uniformData.screenDim = glm::vec2((float)width, (float)height);
  memcpy(graphics.uniformBuffer.mapped, &graphics.uniformData,
         sizeof(Graphics::UniformData));
}

} // namespace VkVP::Particle::Graphics