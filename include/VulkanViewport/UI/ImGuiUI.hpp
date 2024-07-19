#ifndef IMGUI_UI_HPP
#define IMGUI_UI_HPP

#include <GLFW/glfw3.h>
#include <VulkanTools/Buffer.hpp>
#include <VulkanTools/Camera.hpp>
#include <VulkanTools/Device.hpp>
#include <VulkanViewport/Editor/Menu.hpp>
#include <VulkanViewport/Editor/Menu_Window_Defines.hpp>
#include <VulkanViewport/UI/UISettings.hpp>
#include <imgui/imgui.h>
#include <vulkan/vulkan.hpp>
// Options and values to display/toggle from the UI
// ----------------------------------------------------------------------------
// ImGUI class
// ----------------------------------------------------------------------------

namespace VkVP {

struct ImGuiVulkanData {
  ImGuiVulkanData(VulkanDevice *_vulkanDevice) : vulkanDevice(_vulkanDevice) {}
  VkSampler sampler;
  VulkanBuffer vertexBuffer;
  VulkanBuffer indexBuffer;
  int32_t vertexCount = 0;
  int32_t indexCount = 0;
  VkDeviceMemory fontMemory = VK_NULL_HANDLE;
  VkImage fontImage = VK_NULL_HANDLE;
  VkImageView fontView = VK_NULL_HANDLE;
  VkPipelineCache pipelineCache;
  VkPipelineLayout pipelineLayout;
  VkPipeline pipeline;
  VkDescriptorPool descriptorPool;
  VkDescriptorSetLayout descriptorSetLayout;
  VkDescriptorSet descriptorSet;
  VulkanDevice *vulkanDevice;
  std::string title;
  // UI params are set via push constants
  struct PushConstBlock {
    glm::vec2 scale;
    glm::vec2 translate;
  } pushConstBlock;
};

struct ImGuiUI {
  ImGuiVulkanData ivData;
  VulkanDevice *vulkanDevice;
  VkDevice logicalDevice;
  std::string shadersPath;
  std::string fontPath;
  float fontSize = .5f;
  explicit ImGuiUI(VulkanDevice *vulkanDevice,
                   const std::string &shadersPath const std::string &fontPath);

  void initialize(uint32_t width, uint32_t height);

  // Initialize styles, keys, etc.
  void setupVisuals(float width, float height);

  ~ImGuiUI();

  void destroyImGuiVulkanData();

  // Initialize all Vulkan resources used by the ui
  void initializeResources(VkRenderPass &renderPass,
                                      VkQueue copyQueue);

  // Starts a new imGui frame and sets up windows and ui elements
  void newFrame(float frameTime, Camera &camera);

  // Update vertex and index buffer containing the imGui elements when required
  void updateBuffers();

  // Draw current imGui frame into a command buffer
  void drawFrame(VkCommandBuffer commandBuffer);
};

} // namespace VkVP

#endif