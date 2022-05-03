#ifndef NV_VULKAN_UTILS_HPP
#define NV_VULKAN_UTILS_HPP
#include <vulkan/vulkan.hpp>
VkBool32 getSupportedDepthFormat(VkPhysicalDevice physicalDevice, VkFormat *depthFormat);

#endif // NV_VULKAN_UTILS_HPP