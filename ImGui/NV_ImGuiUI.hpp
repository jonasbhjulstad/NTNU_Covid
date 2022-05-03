#ifndef NV_IMGUI_UI_HPP
#define NV_IMGUI_UI_HPP

#include <imgui.h>
#include <unordered_set>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <NV_VulkanBuffer.hpp>
#include <NV_VulkanDevice.hpp>
#include <NV_VulkanInitializers.hpp>
#include <NV_VulkanPipelineInitializers.hpp>
#include <NV_Camera.hpp>
#include "NV_Menu.hpp"
#include "NV_UISettings.hpp"
#include <NV_Assets.hpp>
// Options and values to display/toggle from the UI
// ----------------------------------------------------------------------------
// ImGUI class
// ----------------------------------------------------------------------------

namespace ImGUI_UI
{

struct ImGuiVulkanData
{
	ImGuiVulkanData(VulkanDevice* _vulkanDevice): vulkanDevice(_vulkanDevice){}
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
	struct PushConstBlock
	{
		glm::vec2 scale;
		glm::vec2 translate;
	} pushConstBlock;
};


	void destroyImGuiVulkanData(ImGuiVulkanData& ivData);
	// Initialize styles, keys, etc.
	void setupImGuiVisuals(float width, float height, const UISettings &uiSettings);
	// Initialize all Vulkan resources used by the ui
	void initializeImGuiVulkanResources(ImGuiVulkanData& ivData, VkRenderPass &renderPass, VkQueue copyQueue, const std::string &shadersPath);


	// Starts a new imGui frame and sets up windows and ui elements
	void newFrame(bool updateFrameGraph, UISettings &uiSettings, float frameTime, Camera& camera);

	// Update vertex and index buffer containing the imGui elements when required
	void updateBuffers(VulkanDevice* vulkanDevice, VulkanBuffer& vertexBuffer, VulkanBuffer& indexBuffer,  int32_t& indexCount, int32_t& vertexCount);
	// Draw current imGui frame into a command buffer
	void drawFrame(ImGuiVulkanData& ivData, VkCommandBuffer commandBuffer);
}

#endif