#ifndef NV_ASSETS_HPP
#define NV_ASSETS_HPP
#include <string>
#include "VulkanglTFModel.h"


vkglTF::Model loadModel(const std::string &modelPath, vks::VulkanDevice *vulkanDevice, VkQueue queue)
{
    const uint32_t glTFLoadingFlags = vkglTF::FileLoadingFlags::PreTransformVertices | vkglTF::FileLoadingFlags::PreMultiplyVertexColors | vkglTF::FileLoadingFlags::FlipY;
    vkglTF::Model model;
    model.loadFromFile(modelPath, vulkanDevice, queue, glTFLoadingFlags);
    return model;
}

vks::Texture2D loadTexture(const std::string &texturePath, vks::VulkanDevice *vulkanDevice, VkQueue queue)
{
    vks::Texture2D texture;
    texture.loadFromFile(texturePath, VK_FORMAT_R8G8B8A8_UNORM, vulkanDevice, queue);
    return texture;
}

VkPipelineShaderStageCreateInfo loadShader(
    VkDevice device,
    std::string fileName,
    VkShaderStageFlagBits stage)
{
    VkPipelineShaderStageCreateInfo shaderStage = {};
    shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStage.stage = stage;
    shaderStage.module = vks::tools::loadShader(fileName.c_str(), device);
    shaderStage.pName = "main";
    assert(shaderStage.module != VK_NULL_HANDLE);
    return shaderStage;
}


#endif