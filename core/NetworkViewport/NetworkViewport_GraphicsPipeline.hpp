#ifndef NETWORK_VIEWPORT_GRAPHICS_PIPELINE_HPP
#define NETWORK_VIEWPORT_GRAPHICS_PIPELINE_HPP
#define _USE_MATH_DEFINES
#include <math.h>
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
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include "NetworkViewport_QueueFamilies.hpp"
#include "NetworkViewport_ShaderModule.hpp"
#include "NetworkViewport_SwapChain.hpp"
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

std::vector<VkFramebuffer> createFramebuffers(SwapChainData &swapChainData, VkRenderPass renderPass, VkDevice logicalDevice);
VkRenderPass createRenderPass(VkFormat swapChainImageFormat, VkDevice logicalDevice);

void recreateSwapChain(GLFWwindow *window, VkDevice logicalDevice);







#endif