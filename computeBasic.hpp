#ifndef NV_COMPUTE_BASIC_HPP
#define NV_COMPUTE_BASIC_HPP

#include <vector>
#include <array>
#include "vulkanexamplebase.h"
#include "VulkanglTFModel.h"

namespace BasicCompute

{
    struct BasicComputeParams
    {
        std::string computeShaderPath;
        vks::VulkanDevice *vulkanDevice;
        VkDescriptorPool descriptorPool;
        uint32_t queueFamilyIndex;
        uint32_t N_particles;
        VkPipelineCache pipelineCache = nullptr;
    };

    struct BasicComputePipelineData
    {
        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout pipelineLayout;
        VkDescriptorSet descriptorSet;
        VkCommandPool commandPool;
        VkCommandBuffer commandBuffer;
        VkPipeline pipeline;
        VkDeviceSize *offset;
        uint32_t N_particles;
    };

    static const std::array getPoolSizes()
    {
        return {{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1}};
    }

    VkDescriptorSetLayout setupDescriptorSetLayout(VkDevice device)
    {

        std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {{0}};
        setLayoutBindings[0].binding = 0;
        setLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        setLayoutBindings[0].descriptorCount = 1;
        setLayoutBindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

        setLayoutBindings[1].binding = 1;
        setLayoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        setLayoutBindings[1].descriptorCount = 1;
        setLayoutBindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

        VkDescriptorSetLayoutCreateInfo descriptorLayout = vks::initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
        VkDescriptorSetLayout descriptorSetLayout;
        VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayout, nullptr, &descriptorSetLayout));

        return descriptorSetLayout;
    }

    VkPipelineLayout setupPipelineLayout(VkDevice device)
    {

        VkPipelineLayoutCreateInfo pipelineCI = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
        pipelineCI.setLayoutCount = 1;
        pipelineCI.pSetLayouts = &descriptorSetLayout;

        VkPipelineLayout pipelineLayout;
        VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineCI, nullptr, &pipelineLayout));

        return pipelineLayout;
    }

    VkDescriptorSet setupDescriptorSets(VkDescriptorPool descriptorPool, VkDevice device)
    {
        VkDescriptorSetAllocateInfo allocInfo = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &descriptorSetLayout;

        VkDescriptorSet descriptorSet;
        VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet));

        std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
            vks::initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &uniformParameterBuffer.descriptor),
            vks::initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, &storageBuffer.descriptor)};

        vkUpdateDescriptorSets(device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);

        return descriptorSet;
    }

    VkPipeline preparePipeline(VkDevice device, VkPipelineCache pipelineCache = VK_NULL_HANDLE)
    {
        VkComputePipelineCreateInfo pipelineCI = {VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO};
        VkPipeline pipeline;
        pipelineCI.stage = loadShader(computeShaderPath, VK_SHADER_STAGE_COMPUTE_BIT);
        pipelineCI.layout = pipelineLayout;
        VK_CHECK_RESULT(vkCreateComputePipelines(device, pipelineCache, 1, &pipelineCI, nullptr, &pipeline));

        return pipeline;
    }

    VkCommandPool createCommandPool(uint32_t queueFamilyIndex, VkDevice device)
    {
        VkCommandPool commandPool;
        VkCommandPoolCreateInfo cmdPoolInfo = {};
        cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
        cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        VK_CHECK_RESULT(vkCreateCommandPool(device, &cmdPoolInfo, nullptr, &commandPool));
        return commandPool;
    }

    void createCommandBuffer(vks::VulkanDevice *vulkanDevice, VkCommandPool commandPool)
    {
        commandBuffer = vulkanDevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, commandPool);
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
        VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

        VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &cmdBufInfo));

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1, &descriptorSet, 0, 0);
        vkCmdDispatch(commandBuffer, N_particles / 256, 1, 1);
    }

    std::unique_ptr<BasicComputePipelineData> prepareBasicCompute(const BasicComputeParams &p)
    {
        VkDevice device = p.vulkanDevice->logicalDevice;
        uint32_t computeQueueFamilyIdx = p.vulkanDevice->queueFamilyIndices.compute;

        std::unique_ptr<BasicComputePipelineData> BC_data = std::make_unique<BasicComputePipelineData>();

        p.commandPool = createCommandPool(computeQueueFamilyIdx, device);

        BC_data->descriptorSetLayout = setupDescriptorSetLayout(device);
        BC_data->pipelineLayout = setupPipelineLayout(device);

        BC_data->descriptorSet =  setupDescriptorSets(p.descriptorPool,device);
        BC_data->pipeline = preparePipeline(device, p.pipelineCache);
        BC_data->commandPool = createCommandPool(computeQueueFamilyIdx, device);
        BC_data->commandBuffer = createCommandBuffer(p.vulkanDevice, commandPool);

        return BC_data;
    }
};

#endif
