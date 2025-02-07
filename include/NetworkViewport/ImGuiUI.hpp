#ifndef IMGUI_UI_HPP
#define IMGUI_UI_HPP

#include <imgui/imgui.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <VulkanTools/Buffer.hpp>
#include <VulkanTools/Device.hpp>
#include <VulkanTools/Camera.hpp>
#include <NetworkViewport/Menu_Window_Defines.hpp>
#include <NetworkViewport/Menu.hpp>
#include <NetworkViewport/UISettings.hpp>
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