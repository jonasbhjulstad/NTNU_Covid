#ifndef NV_COMPUTE_BASIC_HPP
#define NV_COMPUTE_BASIC_HPP

#include <vector>
#include <array>
#include "vulkanexamplebase.h"
#include "VulkanglTFModel.h"


template <typename NodeState = glm::vec3>
class ComputeBasic
{
public:
    ComputeBasic(VkDevice _device,
                      vks::VulkanDevice *_vulkanDevice,
                      std::string _computeShaderPath,
                      VkRenderPass _renderPass,
                      uint32_t _N_particles)
        : device(_device),
          vulkanDevice(_vulkanDevice),
          computeShaderPath(_computeShaderPath),
          renderPass(_renderPass),
          N_particles(_N_particles)
    {

        vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);
    }
    uint32_t N_particles;
    uint32_t queueFamilyIndex;
    VkQueue queue;
    VkDevice device;
    vks::Buffer nodeStateBuffer;
    vks::Buffer uniformParameterBuffer;
    VkSemaphore semaphore;
    vks::VulkanDevice *vulkanDevice;
    std::string computeShaderPath;
    VkRenderPass renderPass;
    VkCommandBuffer commandBuffer;
    VkCommandPool commandPool;

    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorSet descriptorSet;


    // static const std::array getPoolSizes()
    // {
        // return {{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1}, {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1}};
    // }

    void setupDescriptorSetLayout()
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
        VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayout, nullptr, &descriptorSetLayout));

        VkPipelineLayoutCreateInfo pipelineCI = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
        pipelineCI.setLayoutCount = 1;
        pipelineCI.pSetLayouts = &descriptorSetLayout;

        VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineCI, nullptr, &pipelineLayout));
    }

    void setupDescriptorSets(VkDescriptorPool descriptorPool)
    {
        VkDescriptorSetAllocateInfo allocInfo = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &descriptorSetLayout;

        VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet));

        std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
            vks::initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &uniformParameterBuffer.descriptor),
            vks::initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, &storageBuffer.descriptor)};

        vkUpdateDescriptorSets(device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
    }

    void preparePipeline(VkPipelineCache pipelineCache = VK_NULL_HANDLE)
    {
        VkComputePipelineCreateInfo pipelineCI = {VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO};

        pipelineCI.stage = loadShader(computeShaderPath, VK_SHADER_STAGE_COMPUTE_BIT);
        pipelineCI.layout = pipelineLayout;
        VK_CHECK_RESULT(vkCreateComputePipelines(device, pipelineCache, 1, &pipelineCI, nullptr, &pipeline));
    }

    void createCommandPool()
    {
        VkCommandPoolCreateInfo cmdPoolInfo = {};
        cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
        cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        VK_CHECK_RESULT(vkCreateCommandPool(device, &cmdPoolInfo, nullptr, &commandPool));
    }

    void createCommandBuffer()
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

    void prepare(std::vector<InstanceData> &instanceData, VkDescriptorPool descriptorPool)
    {
        setupDescriptorSetLayout();
        setupDescriptorSets();
        preparePipeline();
        createCommandPool();
        createCommandBuffer();
        setupDescriptorSets(descriptorPool);
        buildCommandBuffer();
    }

    ~ComputeBasic()
    {
        vkDestroyPipeline(device, pipeline, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
        vkDestroyShaderModule(device, shaderStages[0].module, nullptr);
        vkDestroyShaderModule(device, shaderStages[1].module, nullptr);
    }
};

#endif
