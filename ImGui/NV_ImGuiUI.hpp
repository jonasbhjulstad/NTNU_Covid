#ifndef NV_IMGUI_UI_HPP
#define NV_IMGUI_UI_HPP

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <NV_VulkanBuffer.hpp>
#include <NV_VulkanDevice.hpp>
#include <NV_Camera.hpp>
#include <NV_Menu_Window_Defines.hpp>
#include <NV_Menu.hpp>
#include "NV_UISettings.hpp"
// Options and values to display/toggle from the UI
// ----------------------------------------------------------------------------
// ImGUI class
// ----------------------------------------------------------------------------

namespace ImGUI_UI
{
enum ImGUI_UI_Status
{
	IMGUI_UI_STATUS_NO_ACTION,
	IMGUI_UI_STATUS_NEW_GRAPH
};

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
	void newFrame(UISettings &uiSettings, float frameTime, Camera& camera, igraph_t* graph);

	// Update vertex and index buffer containing the imGui elements when required
	void updateBuffers(VulkanDevice* vulkanDevice, VulkanBuffer& vertexBuffer, VulkanBuffer& indexBuffer,  int32_t& indexCount, int32_t& vertexCount);
	// Draw current imGui frame into a command buffer
	void drawFrame(ImGuiVulkanData& ivData, VkCommandBuffer commandBuffer);
}

#endif