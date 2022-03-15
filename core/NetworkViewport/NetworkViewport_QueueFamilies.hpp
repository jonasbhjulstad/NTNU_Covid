#ifndef NETWORK_VIEWPORT_QUEUE_FAMILIES_HPP
#define NETWORK_VIEWPORT_QUEUE_FAMILIES_HPP
#include <vulkan/vulkan.hpp>
#include <optional>
#include <cstdint>

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);


#endif
