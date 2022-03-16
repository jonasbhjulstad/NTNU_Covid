#ifndef NV_GRAPHICS_PIPELINE_HPP
#define NV_GRAPHICS_PIPELINE_HPP
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <array>
#include <set>
#include "NV_Queue.hpp"
#include "NV_ShaderModule.hpp"
#include "NV_SwapChain.hpp"
#include "NV_Buffers.hpp"
// #include <imgui_impl_vulkan.h>

struct PipelineData
{
    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;
    GLFWwindow* window;
    VkPipeline pipeline;
    VkPipelineLayout layout;
    SwapChainData swapChainData;
    VkRenderPass renderPass;
};




void createImageViews(SwapChainData &swapChainData, VkDevice logicalDevice);

VkRenderPass createRenderPass(VkFormat swapChainImageFormat, VkDevice logicalDevice);

void recreateSwapChain(GLFWwindow *window, VkDevice logicalDevice);







#endif