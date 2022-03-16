#ifndef NV_SWAPCHAIN_HPP
#define NV_SWAPCHAIN_HPP
#include <vulkan/vulkan.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <limits>
#include "NV_Queue.hpp"

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};
struct SwapChainData
{
    VkSwapchainKHR swapChain;
    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
    VkFormat imageFormat;
    VkExtent2D extent;
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
    SwapChainSupportDetails support;
};



SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window);

SwapChainData createSwapChain(VkPhysicalDevice physicalDevice,
                              const SwapChainSupportDetails &swapChainSupport,
                              VkSurfaceKHR surface,
                              GLFWwindow *window,
                              VkDevice logicalDevice);

void cleanupSwapChain(SwapChainData &swapChainData,
                      std::vector<VkFramebuffer> &frameBuffers,
                      VkPipeline graphicsPipeline,
                      VkPipelineLayout pipelineLayout,
                      VkRenderPass renderPass,
                      VkDevice logicalDevice);

#endif