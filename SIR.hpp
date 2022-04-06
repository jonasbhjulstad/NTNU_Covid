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
    glTFBasicInstance()
    {
    }

    VkDevice device;
    vks::VulkanDevice* vulkanDevice;
    vks::Buffer* uniformProjectionBuffer;
    VkQueue graphicsQueue;
    glTFBasicInstance bezier;
    glTFBasicInstance node;
    ComputeBasic compute;

    void prepare()
    {

    }


};

#endif