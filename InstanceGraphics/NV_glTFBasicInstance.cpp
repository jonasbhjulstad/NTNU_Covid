#include "NV_glTFBasicInstance.hpp"
#include "NV_NodeInstance.hpp"
#include "NV_EdgeInstance.hpp"
namespace glTFBasicInstance
{
    template <> VulkanBuffer prepareInstanceBuffer<NodeInstanceData>(std::vector<NodeInstanceData> &instanceData, VulkanDevice *vulkanDevice, VkQueue queue);
    template <> VulkanBuffer prepareInstanceBuffer<EdgeInstanceData>(std::vector<EdgeInstanceData> &instanceData, VulkanDevice *vulkanDevice, VkQueue queue);
    
        template <>VkPipeline setupPipeline<NodeInstanceData>(const std::string &vertexShaderPath,
                             const std::string &fragmentShaderPath,
                             VkPipelineLayout pipelineLayout,
                             std::vector<VkPipelineShaderStageCreateInfo> &shaderStages,
                             VkDevice device,
                             VkRenderPass renderPass,
                             VkPipelineCache pipelineCache);

                                     template <>VkPipeline setupPipeline<EdgeInstanceData>(const std::string &vertexShaderPath,
                             const std::string &fragmentShaderPath,
                             VkPipelineLayout pipelineLayout,
                             std::vector<VkPipelineShaderStageCreateInfo> &shaderStages,
                             VkDevice device,
                             VkRenderPass renderPass,
                             VkPipelineCache pipelineCache = VK_NULL_HANDLE);
 

    template <> std::unique_ptr<InstancePipelineData> prepareBasicInstancedRendering<NodeInstanceData>(BasicInstancedRenderingParams &p);
    template <> std::unique_ptr<InstancePipelineData> prepareBasicInstancedRendering<EdgeInstanceData>(BasicInstancedRenderingParams &p);

    std::vector<VkDescriptorPoolSize> getPoolSizes()
    {
        return {initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1),
                initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1)};
    }

    VkDescriptorSetLayout uniformDescriptorSetLayout(VkDevice device)
    {

        std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {
            // Binding 0 : Vertex shader uniform buffer
            initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
        };

        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorSetLayoutCreateInfo descriptorLayout = initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
        VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayout, nullptr, &descriptorSetLayout));

        return descriptorSetLayout;
    }

    VkDescriptorSetLayout textureDescriptorSetLayout(VkDevice device)
    {

        std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {
            // Binding 0 : Vertex shader uniform buffer
            initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
            initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)};

        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorSetLayoutCreateInfo descriptorLayout = initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
        VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayout, nullptr, &descriptorSetLayout));

        return descriptorSetLayout;
    }

    VkPipelineLayout setupPipelineLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout)
    {
        VkPipelineLayout pipelineLayout;
        // Pipeline layout
        VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = initializers::pipelineLayoutCreateInfo(&descriptorSetLayout, 1);
        VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pPipelineLayoutCreateInfo, nullptr, &pipelineLayout));
        return pipelineLayout;
    }

    VkDescriptorSet setupDescriptorSets(VkDevice device,
                                        VkDescriptorSetLayout descriptorSetLayout,
                                        VkDescriptorPool descriptorPool,
                                        VulkanBuffer *uniformProjectionBuffer,
                                        Texture* texture)
    {
        VkDescriptorSet descriptorSet;
        VkDescriptorSetAllocateInfo allocInfo = initializers::descriptorSetAllocateInfo(descriptorPool, &descriptorSetLayout, 1);

        VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet));

        std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
            initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &uniformProjectionBuffer->descriptor)};
        if (texture)
        {
            writeDescriptorSets.push_back(initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &texture->descriptor));
        }
        vkUpdateDescriptorSets(device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);

        return descriptorSet;
    }

    void buildCommandBuffer(InstancePipelineData &BI_data, VkCommandBuffer commandBuffer)
    {
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, BI_data.pipelineLayout, 0, 1, &BI_data.descriptorSet, 0, nullptr);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, BI_data.pipeline);

        vkCmdBindVertexBuffers(commandBuffer, GLTF_BIP_VERTEX_BIND_ID, 1, &BI_data.model->vertices.buffer, BI_data.offset);
        vkCmdBindVertexBuffers(commandBuffer, GLTF_BIP_INSTANCE_BIND_ID, 1, &BI_data.instanceBuffer.buffer, BI_data.offset);

        vkCmdBindIndexBuffer(commandBuffer, BI_data.model->indices.buffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdDrawIndexed(commandBuffer, BI_data.model->indices.count, BI_data.N_instances, 0, 0, 0);
    }




}