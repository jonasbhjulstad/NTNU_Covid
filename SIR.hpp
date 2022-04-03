#ifndef GLTF_BASIC_INSTANCE_PIPELINE_HPP
#define GLTF_BASIC_INSTANCE_PIPELINE_HPP
#include <vector>
#include <array>
#include "vulkanexamplebase.h"
#include "VulkanglTFModel.h"
#include "gltfBasicInstance.hpp"
#include "computeBasic.hpp"

template <typename InstanceData = glm::vec3>
class glTFBasicInstance
{
public:
    glTFBasicInstance(VkDevice _device, 
    vks::VulkanDevice* _vulkanDevice, 
    vks::Buffer* _uniformProjectionBuffer, 
    VkQueue _queue,
    std::string _vertexShaderPath,
    std::string _fragmentShaderPath,
    std::string _modelAssetPath,
    VkRenderPass _renderPass,
    vkglTF::Model* _model,
    vks::Texture2DArray* _texture = nullptr)
        : device(_device),
          vulkanDevice(_vulkanDevice),
          uniformProjectionBuffer(_uniformProjectionBuffer),
          graphicsQueue(_queue),
          renderPass(_renderPass),
          model(_model)
    {
    }

    VkDevice device;
    vks::VulkanDevice* vulkanDevice;
    vks::Buffer* uniformProjectionBuffer;
    VkQueue graphicsQueue;
    glTFBasicInstance bezier;
    glTFBasicInstance node;
    ComputeBasic compute;

    constexpr std::array<VkPoolSize, 2> getPoolSizes()
    {
       std::vector<VkPoolSize> poolSizes;
       poolSizes.reserve(6);
       poolSizes.push_back(glTFBasicInstance::getPoolSizes()); 
       poolSizes.push_back(glTFBasicInstance::getPoolSizes()); 
       poolSizes.push_back(ComputeBasic::getPoolSizes()); 
    }

    void prepare()
    {

    }


};

#endif