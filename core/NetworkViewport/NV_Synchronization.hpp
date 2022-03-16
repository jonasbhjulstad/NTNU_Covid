#ifndef NV_SYNCHRONIZATION_HPP
#define NV_SYNCHRONIZATION_HPP
#include <vector>
#include <vulkan/vulkan.hpp>

struct FrameSemaphores
{
    FrameSemaphores(const uint32_t size)
    {
        imageAvailable.resize(size);
        renderFinished.resize(size);
        fence.resize(size);
    }
    std::vector<VkSemaphore> imageAvailable;
    std::vector<VkSemaphore> renderFinished;
    std::vector<VkFence> fence;
};

#endif