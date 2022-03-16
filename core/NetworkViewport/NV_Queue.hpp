#ifndef NV_QUEUE_HPP
#define NV_QUEUE_HPP
#include <set>
#include <optional>
#include <cstdint>
#include <vulkan/vulkan.hpp>

struct QueueData
{
    VkDevice logicalDevice;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
};

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
