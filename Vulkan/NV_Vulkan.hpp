#ifndef NV_VULKAN_HPP
#define NV_VULKAN_HPP
#include <iostream>
#include <string>
#include <vulkan/vulkan.hpp>


std::string vulkanErrorString(VkResult errorCode);
std::string physicalDeviceTypeString(VkPhysicalDeviceType type);

#define VK_CHECK_RESULT(f)                                                                                                                    \
	{                                                                                                                                         \
		VkResult res = (f);                                                                                                                   \
		if (res != VK_SUCCESS)                                                                                                                \
		{                                                                                                                                     \
			std::cout << "Fatal : VkResult is \"" << vulkanErrorString(res) << "\" in " << __FILE__ << " at line " << __LINE__ << "\n"; \
			assert(res == VK_SUCCESS);                                                                                                        \
		}                                                                                                                                     \
	}

#endif