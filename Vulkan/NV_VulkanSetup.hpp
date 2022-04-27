#ifndef NV_VULKAN_SETUP_HPP
#define NV_VULKAN_SETUP_HPP
#include <string>
#include <iostream>
#include "NV_Vulkan.hpp"
#include "NV_VulkanInstance.hpp"
#include "NV_VulkanDebug.hpp"
#include "NV_VulkanSwapChain.hpp"
#include "NV_VulkanInitializers.hpp"
#include "NV_VulkanPipelineInitializers.hpp"
#include "NV_VulkanUtils.hpp"

VkResult createVulkanInstance(bool enableValidation,
							  const std::string &name,
							  VkInstance& instance,
							  std::vector<std::string> &supportedInstanceExtensions,
							  std::vector<const char *> &enabledInstanceExtensions,
							  uint32_t apiVersion);
void setupVulkanPhysicalDevice(VulkanInstance &vulkanInstance, bool enableValidation);

void prepareVulkan(VulkanInstance &vulkanInstance, uint32_t width, uint32_t height);

#endif