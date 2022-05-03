#include "NV_Assets.hpp"
std::unique_ptr<vkglTF::Model> loadModel(const std::string &modelPath, VulkanDevice *vulkanDevice, VkQueue queue)
{
    const uint32_t glTFLoadingFlags = vkglTF::FileLoadingFlags::PreTransformVertices | vkglTF::FileLoadingFlags::PreMultiplyVertexColors | vkglTF::FileLoadingFlags::FlipY;
    std::unique_ptr<vkglTF::Model> model = std::make_unique<vkglTF::Model>();
    model->loadFromFile(modelPath, vulkanDevice, queue, glTFLoadingFlags);
    return model;
}

std::unique_ptr<Texture2D> loadTexture(const std::string &texturePath, VulkanDevice *vulkanDevice, VkQueue queue)
{
    std::unique_ptr<Texture2D> texture = std::make_unique<Texture2D>();
    texture->loadFromFile(texturePath, VK_FORMAT_R8G8B8A8_UNORM, vulkanDevice, queue);
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
    shaderStage.module = tools::loadShaderModule(fileName.c_str(), device);
    shaderStage.pName = "main";
    assert(shaderStage.module != VK_NULL_HANDLE);
    return shaderStage;
}