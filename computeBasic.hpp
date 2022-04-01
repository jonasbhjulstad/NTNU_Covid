#ifndef NV_COMPUTE_BASIC_HPP
#define NV_COMPUTE_BASIC_HPP

#include <vector>
#include <array>
#include "vulkanexamplebase.h"
#include "VulkanglTFModel.h"

class ComputeBasic
{
public:
    glTFBasicInstance(VkDevice _device,
                      vks::VulkanDevice *_vulkanDevice,
                      vks::Buffer *_uniformProjectionBuffer,
                      VkQueue _queue,
                      std::string _vertexShaderPath,
                      std::string _fragmentShaderPath,
                      std::string _modelAssetPath,
                      VkRenderPass _renderPass,
                      vkglTF::Model *_model,
                      vks::Texture2DArray *_texture = nullptr)
        : device(_device),
          vulkanDevice(_vulkanDevice),
          uniformProjectionBuffer(_uniformProjectionBuffer),
          queue(_queue),
          vertexShaderPath(_vertexShaderPath),
          fragmentShaderPath(_fragmentShaderPath),
          modelAssetPath(_modelAssetPath),
          renderPass(_renderPass),
          model(_model),
          texture(_texture)
    {
    }
    VkQueue queue;
    VkDevice device;
    vks::VulkanDevice *vulkanDevice;
    vkglTF::Model *model;
    vks::Texture2DArray *texture;
    vks::Buffer *uniformProjectionBuffer;
    std::string vertexShaderPath;
    std::string fragmentShaderPath;
    std::string modelAssetPath;
    VkRenderPass renderPass;

    const enum bind_IDs { GLTF_BIP_VERTEX_BIND_ID,
                          GLTF_BIP_INSTANCE_BIND_ID };

    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorSet descriptorSet;
    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages;

    vks::Buffer instanceBuffer;

    uint32_t N_instances = 0;

    static const VkDescriptorPoolSize[2] getPoolSizes()
    {
        return {{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1}, {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1}};
    }

    void setupDescriptorSetLayout()
    {

        std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings[2] = {{0}};
        setLayoutBindings[0].binding = 0;
        setLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        setLayoutBindings.descriptorCount = 1;
        setLayoutBindings.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

        setLayoutBindings[1].binding = 1;
        setLayoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        setLayoutBindings[1].descriptorCount = 1;
        setLayoutBindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

        VkDescriptorSetLayoutCreateInfo descriptorLayout = vks::initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
        VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayout, nullptr, &descriptorSetLayout));

        VkPipelineLayoutCreateInfo pipelineCI = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
        pipelineCI.setLayoutCount = 1;
        pipelineCI.pSetLayouts = &descriptorSetLayout;

        VK_CHECK_RESULT(vkCreatePipelineLayout(device, &layoutInfo, nullptr, &pipelineLayout));
    }

    void setupDescriptorSets(VkDescriptorPool descriptorPool)
    {
        VkDescriptorSetAllocateInfo allocInfo = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &setLayout;

        VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet));

        std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
            vks::initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &uniformProjectionBuffer->descriptor),
            vks::initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &texture->descriptor)};

        vkUpdateDescriptorSets(device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
    }

    void preparePipeline(VkPipelineCache pipelineCache = VK_NULL_HANDLE)
    {
        VkComputePipelineCreateInfo pipelineCI = {VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO};

        VkPipelineShaderStageCreateInfo shaderStage = loadShader(computeShaderPath, VK_SHADER_STAGE_COMPUTE_BIT);

        pipelineCI.stage = shaderStage;
        pipelineCI.layout = pipelineLayout;

        VkPipelineColorBlendAttachmentState blendAttachment = {0};
        blendAttachment.blendEnable = true;
        blendAttachment.colorWriteMask = 0xf;
        blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
        blendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        blendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    }

    VkPipelineShaderStageCreateInfo loadShader(std::string fileName, VkShaderStageFlagBits stage)
    {
        VkPipelineShaderStageCreateInfo shaderStage = {};
        shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStage.stage = stage;
        shaderStage.module = vks::tools::loadShader(fileName.c_str(), device);
        shaderStage.pName = "main";
        assert(shaderStage.module != VK_NULL_HANDLE);
        return shaderStage;
    }

    void buildCommandBuffer(VkCommandBuffer drawCmdBuffer, VkDeviceSize *offsets)
    {
        VkMemoryBarrier compVertBarrier{};
        memoryBarrier()
        compVertBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
        compVertBarrier.srcAccessMask = VK_PIPELINE_STAGE_VERTEX_COMPUTE_SHADER_BIT;
        compVertBarrier.dstAccessMask = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;

        vkCmdBindPipeline(drawCmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);
        vkCmdBindDescriptorSets(drawCmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

        VkCmdDispatch(cmd, (positionBuffer.size / sizeof(glm::vec2)),  N_PARTICLES_PER_WORKGROUP, 1, 1);



    }

    void prepare(std::vector<InstanceData> &instanceData, VkDescriptorPool descriptorPool)
    {
        prepareInstanceData(instanceData);
        setupDescriptorSetLayout();
        preparePipeline();
        setupDescriptorSets(descriptorPool);
    }

    ~glTFBasicInstance()
    {
        vkDestroyPipeline(device, pipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
        vkDestroyShaderModule(device, shaderStages[0].module, nullptr);
        vkDestroyShaderModule(device, shaderStages[1].module, nullptr);
    }
};

#endif
