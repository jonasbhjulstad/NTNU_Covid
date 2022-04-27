#ifndef GLTF_BASIC_INSTANCE_PIPELINE_HPP
#define GLTF_BASIC_INSTANCE_PIPELINE_HPP
#include <vector>
#include "vulkanexamplebase.h"
#include "VulkanglTFModel.h"
#include "NV_Assets.hpp"

enum glTFBasicInstanceBindIDs
{
    GLTF_BIP_VERTEX_BIND_ID,
    GLTF_BIP_INSTANCE_BIND_ID
};

struct BasicInstancePipelineData
{
    std::unique_ptr<vkglTF::Model> model;
    std::unique_ptr<vks::Texture2D> texture = nullptr;
    vks::Buffer instanceBuffer;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkDescriptorSet descriptorSet;
    VkPipeline pipeline;
    VkDeviceSize* offset;
    uint32_t N_instances = 0;
    VkDevice device;
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

    BasicInstancePipelineData(VkDevice _device): device(_device)
    {
        shaderStages.reserve(2);
    }

    ~BasicInstancePipelineData()
    {
        vkDestroyPipeline(device, pipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
        if (texture != nullptr)
        {
            texture->destroy();
        }
        
        for (auto& stage: shaderStages)
        {
            vkDestroyShaderModule(device, stage.module, nullptr);
        }
    }
};

struct BasicInstancedRenderingParams
{
    std::string vertexShaderPath;
    std::string fragmentShaderPath;
    std::string modelPath;
    std::string texturePath;
    vks::VulkanDevice *vulkanDevice;
    vks::Buffer *uniformProjectionBuffer;
    VkQueue queue;
    VkDescriptorPool descriptorPool;
    VkRenderPass renderPass;
    VkPipelineCache pipelineCache = nullptr;
};

template <typename InstanceData>
vks::Buffer prepareInstanceBuffer(std::vector<InstanceData> &instanceData, vks::VulkanDevice* vulkanDevice, VkQueue queue)
{
    vks::Buffer instanceBuffer;
    uint32_t N_instances = static_cast<uint32_t>(instanceData.size());
    instanceBuffer.size = N_instances * sizeof(InstanceData);

    struct
    {
        VkDeviceMemory memory;
        VkBuffer buffer;
    } stagingBuffer;

    VK_CHECK_RESULT(vulkanDevice->createBuffer(
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        instanceBuffer.size,
        &stagingBuffer.buffer,
        &stagingBuffer.memory,
        instanceData.data()));

    VK_CHECK_RESULT(vulkanDevice->createBuffer(
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        instanceBuffer.size,
        &instanceBuffer.buffer,
        &instanceBuffer.memory));

    VkCommandBuffer copyCmd = vulkanDevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

    VkBufferCopy copyRegion = {};
    copyRegion.size = instanceBuffer.size;
    vkCmdCopyBuffer(
        copyCmd,
        stagingBuffer.buffer,
        instanceBuffer.buffer,
        1,
        &copyRegion);

    vulkanDevice->flushCommandBuffer(copyCmd, queue, true);

    instanceBuffer.descriptor.range = instanceBuffer.size;
    instanceBuffer.descriptor.buffer = instanceBuffer.buffer;
    instanceBuffer.descriptor.offset = 0;

    // Destroy staging resources
    vkDestroyBuffer(vulkanDevice->logicalDevice, stagingBuffer.buffer, nullptr);
    vkFreeMemory(vulkanDevice->logicalDevice, stagingBuffer.memory, nullptr);

    return instanceBuffer;
}

std::vector<VkDescriptorPoolSize> getPoolSizes()
{
    return {vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1),
            vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)};
}

VkDescriptorSetLayout uniformDescriptorSetLayout(VkDevice device)
{

    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {
        // Binding 0 : Vertex shader uniform buffer
        vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
    };

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorSetLayoutCreateInfo descriptorLayout = vks::initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayout, nullptr, &descriptorSetLayout));

    return descriptorSetLayout;
}

VkDescriptorSetLayout textureDescriptorSetLayout(VkDevice device)
{

    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {
        // Binding 0 : Vertex shader uniform buffer
        vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
        vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)};

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorSetLayoutCreateInfo descriptorLayout = vks::initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayout, nullptr, &descriptorSetLayout));

    return descriptorSetLayout;
}

VkPipelineLayout setupPipelineLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout)
{
    VkPipelineLayout pipelineLayout;
    // Pipeline layout
    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = vks::initializers::pipelineLayoutCreateInfo(&descriptorSetLayout, 1);
    VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pPipelineLayoutCreateInfo, nullptr, &pipelineLayout));
    return pipelineLayout;
}

VkDescriptorSet setupDescriptorSets(VkDevice device,
                                    VkDescriptorSetLayout descriptorSetLayout,
                                    VkDescriptorPool descriptorPool,
                                    vks::Buffer *uniformProjectionBuffer,
                                    vks::Texture *texture = nullptr)
{
    VkDescriptorSet descriptorSet;
    VkDescriptorSetAllocateInfo allocInfo = vks::initializers::descriptorSetAllocateInfo(descriptorPool, &descriptorSetLayout, 1);

    VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet));

    std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
        vks::initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &uniformProjectionBuffer->descriptor)};
    if (texture)
    {
        writeDescriptorSets.push_back(vks::initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &texture->descriptor));
    }
    vkUpdateDescriptorSets(device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);

    return descriptorSet;
}

template <typename InstanceData>
VkPipeline setupPipeline(const std::string &vertexShaderPath,
                         const std::string &fragmentShaderPath,
                         VkPipelineLayout pipelineLayout,
                         std::vector<VkPipelineShaderStageCreateInfo>& shaderStages,
                         VkDevice device,
                         VkRenderPass renderPass,
                         VkPipelineCache pipelineCache = VK_NULL_HANDLE)
{
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = vks::initializers::pipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);
    VkPipelineRasterizationStateCreateInfo rasterizationState = vks::initializers::pipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
    VkPipelineColorBlendAttachmentState blendAttachmentState = vks::initializers::pipelineColorBlendAttachmentState(0xf, VK_FALSE);
    VkPipelineColorBlendStateCreateInfo colorBlendState = vks::initializers::pipelineColorBlendStateCreateInfo(1, &blendAttachmentState);
    VkPipelineDepthStencilStateCreateInfo depthStencilState = vks::initializers::pipelineDepthStencilStateCreateInfo(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL);
    VkPipelineViewportStateCreateInfo viewportState = vks::initializers::pipelineViewportStateCreateInfo(1, 1, 0);
    VkPipelineMultisampleStateCreateInfo multisampleState = vks::initializers::pipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT);
    std::vector<VkDynamicState> dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamicState = vks::initializers::pipelineDynamicStateCreateInfo(dynamicStateEnables);

    VkGraphicsPipelineCreateInfo pipelineCI = vks::initializers::pipelineCreateInfo(pipelineLayout, renderPass);
    pipelineCI.pInputAssemblyState = &inputAssemblyState;
    pipelineCI.pRasterizationState = &rasterizationState;
    pipelineCI.pColorBlendState = &colorBlendState;
    pipelineCI.pMultisampleState = &multisampleState;
    pipelineCI.pViewportState = &viewportState;
    pipelineCI.pDepthStencilState = &depthStencilState;
    pipelineCI.pDynamicState = &dynamicState;
    pipelineCI.stageCount = static_cast<uint32_t>(2);
    pipelineCI.pStages = shaderStages.data();

    // Vertex input bindings
    // The instancing pipeline uses a vertex input state with two bindings
    std::vector<VkVertexInputBindingDescription> bindingDescriptions = {
        // Binding point 0: Mesh vertex layout description at per-vertex rate
        initializers::InputBindingDescription(GLTF_BIP_VERTEX_BIND_ID, sizeof(vkglTF::Vertex), VK_VERTEX_INPUT_RATE_VERTEX),
        // Binding point 1: Instanced data at per-instance rate
        initializers::InputBindingDescription(GLTF_BIP_INSTANCE_BIND_ID, sizeof(InstanceData), VK_VERTEX_INPUT_RATE_INSTANCE)};

    std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
        initializers::InputAttributeDescription(GLTF_BIP_VERTEX_BIND_ID, 0, VK_FORMAT_R32G32B32_SFLOAT, 0),                 // Location 0: Position
        initializers::InputAttributeDescription(GLTF_BIP_VERTEX_BIND_ID, 1, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float) * 3), // Location 1: Normal
        initializers::InputAttributeDescription(GLTF_BIP_VERTEX_BIND_ID, 2, VK_FORMAT_R32G32_SFLOAT, sizeof(float) * 6),    // Location 2: Texture coordinates
        initializers::InputAttributeDescription(GLTF_BIP_VERTEX_BIND_ID, 3, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float) * 8), // Location 3: Color
    };

    std::vector<VkVertexInputAttributeDescription> instanceDataDescriptions = InstanceData::getAttributeDescriptions(GLTF_BIP_INSTANCE_BIND_ID, 4);

    attributeDescriptions.insert(attributeDescriptions.end(), instanceDataDescriptions.begin(), instanceDataDescriptions.end());
    VkPipelineVertexInputStateCreateInfo inputState = vks::initializers::pipelineVertexInputStateCreateInfo();
    inputState.pVertexBindingDescriptions = bindingDescriptions.data();
    inputState.pVertexAttributeDescriptions = attributeDescriptions.data();

    inputState.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
    inputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    pipelineCI.pVertexInputState = &inputState;

    shaderStages.push_back(loadShader(device, vertexShaderPath, VK_SHADER_STAGE_VERTEX_BIT));
    shaderStages.push_back(loadShader(device, fragmentShaderPath, VK_SHADER_STAGE_FRAGMENT_BIT));

    VkPipeline pipeline;
    VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, nullptr, 1, &pipelineCI, nullptr, &pipeline));
    return pipeline;
}

void buildCommandBuffer(BasicInstancePipelineData& BI_data, VkCommandBuffer commandBuffer)
{
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, BI_data.pipelineLayout, 0, 1, &BI_data.descriptorSet, 0, nullptr);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, BI_data.pipeline);

    vkCmdBindVertexBuffers(commandBuffer, GLTF_BIP_VERTEX_BIND_ID, 1, &BI_data.model->vertices.buffer, BI_data.offset);
    vkCmdBindVertexBuffers(commandBuffer, GLTF_BIP_INSTANCE_BIND_ID, 1, &BI_data.instanceBuffer.buffer, BI_data.offset);

    vkCmdBindIndexBuffer(commandBuffer, BI_data.model->indices.buffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(commandBuffer, BI_data.model->indices.count, BI_data.N_instances, 0, 0, 0);
}

template <typename InstanceData>
std::unique_ptr<BasicInstancePipelineData> prepareBasicInstancedRendering(BasicInstancedRenderingParams &p)
{

    VkDevice device = p.vulkanDevice->logicalDevice;

    std::unique_ptr<BasicInstancePipelineData> BI_data = std::make_unique<BasicInstancePipelineData>(device);

    BI_data->model = loadModel(p.modelPath, p.vulkanDevice, p.queue);
    // BI_data->instanceBuffer = prepareInstanceBuffer(instanceData, p.vulkanDevice, p.queue);
    if (!p.texturePath.empty())
    {
        BI_data->texture = loadTexture(p.texturePath, p.vulkanDevice, p.queue);
        BI_data->descriptorSetLayout = textureDescriptorSetLayout(device);
    }
    else
    {
        BI_data->descriptorSetLayout = uniformDescriptorSetLayout(device);
    }
    BI_data->pipelineLayout = setupPipelineLayout(device, BI_data->descriptorSetLayout);
    BI_data->descriptorSet = setupDescriptorSets(device, BI_data->descriptorSetLayout, p.descriptorPool, p.uniformProjectionBuffer);
    BI_data->pipeline = setupPipeline<InstanceData>(p.vertexShaderPath, p.fragmentShaderPath, BI_data->pipelineLayout, BI_data->shaderStages, device, p.renderPass, p.pipelineCache);

    return BI_data;
}

#endif