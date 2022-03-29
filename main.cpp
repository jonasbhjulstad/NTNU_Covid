/*
 * Vulkan Example - imGui (https://github.com/ocornut/imgui)
 *
 * Copyright (C) 2017 by Sascha Willems - www.saschawillems.de
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <imgui.h>
#include "vulkanexamplebase.h"
#include "VulkanglTFModel.h"
#include <random>
#include <chrono>
#define ENABLE_VALIDATION true

#include "ImGuiApp.hpp"
// ----------------------------------------------------------------------------
// VulkanExample
// ----------------------------------------------------------------------------
#define NODE_VERTEX_BIND_ID 0
#define NODE_INSTANCE_BIND_ID 1
#define NODE_INSTANCE_COUNT 10
#define BEZIER_LINE_WIDTH 10.f
class VulkanExample : public VulkanExampleBase
{
public:
	ImGUI *imGui = nullptr;
	struct Models
	{
		vkglTF::Model models;
		vkglTF::Model logos;
		vkglTF::Model background;
		vkglTF::Model node;
	} models;


	struct NodeInstanceData
	{
		glm::vec3 pos;
		glm::vec3 rot;
	};

	vks::Buffer nodeInstanceBuffer;

	VulkanExample() : VulkanExampleBase(ENABLE_VALIDATION)
	{
		title = "Vulkan Example - ImGui";
		camera.type = Camera::CameraType::lookat;
		camera.setPosition(glm::vec3(0.0f, 0.0f, -4.8f));
		camera.setRotation(glm::vec3(4.5f, -380.0f, 0.0f));
		camera.setPerspective(45.0f, (float)width / (float)height, 0.1f, 256.0f);
		// Don't use the ImGui overlay of the base framework in this sample
		settings.overlay = false;

		this->enabledFeatures.fillModeNonSolid = true;

	}

	~VulkanExample()
	{
		// vkDestroyPipeline(device, pipelines.statue, nullptr);
		// vkDestroyPipeline(device, pipelines.node, nullptr);
		// vkDestroyPipelineLayout(device, pipelineLayouts.statue, nullptr);
		// vkDestroyPipelineLayout(device, pipelineLayouts.node, nullptr);
		// vkDestroyPipelineLayout(device, pipelineLayouts.bezier, nullptr);
		// vkDestroyDescriptorSetLayout(device, descriptorSetLayouts.statue, nullptr);
		// vkDestroyDescriptorSetLayout(device, descriptorSetLayouts.node, nullptr);

		// uniformBuffers.VS.destroy();

		delete imGui;
	}

	void buildCommandBuffers()
	{
		VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

		VkClearValue clearValues[2];
		clearValues[0].color = {{0.2f, 0.2f, 0.2f, 1.0f}};
		clearValues[1].depthStencil = {1.0f, 0};

		VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
		renderPassBeginInfo.renderPass = renderPass;
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = width;
		renderPassBeginInfo.renderArea.extent.height = height;
		renderPassBeginInfo.clearValueCount = 2;
		renderPassBeginInfo.pClearValues = clearValues;

		imGui->newFrame(this, (frameCounter == 0));

		imGui->updateBuffers();

		for (int32_t i = 0; i < drawCmdBuffers.size(); ++i)
		{
			// Set target frame buffer
			renderPassBeginInfo.framebuffer = frameBuffers[i];

			VK_CHECK_RESULT(vkBeginCommandBuffer(drawCmdBuffers[i], &cmdBufInfo));

			vkCmdBeginRenderPass(drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

			VkViewport viewport = vks::initializers::viewport((float)width, (float)height, 0.0f, 1.0f);
			vkCmdSetViewport(drawCmdBuffers[i], 0, 1, &viewport);

			VkRect2D scissor = vks::initializers::rect2D(width, height, 0, 0);
			vkCmdSetScissor(drawCmdBuffers[i], 0, 1, &scissor);


			// Render imGui
			imGui->drawFrame(drawCmdBuffers[i]);

			vkCmdEndRenderPass(drawCmdBuffers[i]);

			VK_CHECK_RESULT(vkEndCommandBuffer(drawCmdBuffers[i]));
		}
	}

	void setupDescriptorPool()
	{
		std::vector<VkDescriptorPoolSize> poolSizes = {};

		VkDescriptorPoolCreateInfo descriptorPoolInfo = vks::initializers::descriptorPoolCreateInfo(poolSizes, 2);
		VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &descriptorPool));
	}

	void setupDescriptorSetLayouts()
	{
		// Statue
		std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {
			vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
		};
		VkDescriptorSetLayoutCreateInfo descriptorLayout =
			vks::initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
		VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayout, nullptr, &descriptorSetLayouts.statue));

		// Pipeline layout
		VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = vks::initializers::pipelineLayoutCreateInfo(&descriptorSetLayouts.statue, 1);
		VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pPipelineLayoutCreateInfo, nullptr, &pipelineLayouts.statue));

	}

	void setupDescriptorSets()
	{

		// Statue
		VkDescriptorSetAllocateInfo allocInfo = vks::initializers::descriptorSetAllocateInfo(descriptorPool, &descriptorSetLayouts.statue, 1);
		VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSets.statue));

		std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
			vks::initializers::writeDescriptorSet(descriptorSets.statue, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &uniformBuffers.VS.descriptor)};
		vkUpdateDescriptorSets(device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);

		allocInfo = vks::initializers::descriptorSetAllocateInfo(descriptorPool, &descriptorSetLayouts.node, 1);

		// node

		VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSets.node));

		writeDescriptorSets = {
			vks::initializers::writeDescriptorSet(descriptorSets.node, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &uniformBuffers.VS.descriptor)};

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);

	}

	void preparePipelines()
	{
		// Rendering
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = vks::initializers::pipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);
		VkPipelineRasterizationStateCreateInfo rasterizationState = vks::initializers::pipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
		VkPipelineColorBlendAttachmentState blendAttachmentState = vks::initializers::pipelineColorBlendAttachmentState(0xf, VK_FALSE);
		VkPipelineColorBlendStateCreateInfo colorBlendState = vks::initializers::pipelineColorBlendStateCreateInfo(1, &blendAttachmentState);
		VkPipelineDepthStencilStateCreateInfo depthStencilState = vks::initializers::pipelineDepthStencilStateCreateInfo(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL);
		VkPipelineViewportStateCreateInfo viewportState = vks::initializers::pipelineViewportStateCreateInfo(1, 1, 0);
		VkPipelineMultisampleStateCreateInfo multisampleState = vks::initializers::pipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT);
		std::vector<VkDynamicState> dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamicState = vks::initializers::pipelineDynamicStateCreateInfo(dynamicStateEnables);

		std::array<VkPipelineShaderStageCreateInfo, 3> shaderStages;


		VkGraphicsPipelineCreateInfo pipelineCI = vks::initializers::pipelineCreateInfo(pipelineLayouts.statue, renderPass);
		pipelineCI.pInputAssemblyState = &inputAssemblyState;
		pipelineCI.pRasterizationState = &rasterizationState;
		pipelineCI.pColorBlendState = &colorBlendState;
		pipelineCI.pMultisampleState = &multisampleState;
		pipelineCI.pViewportState = &viewportState;
		pipelineCI.pDepthStencilState = &depthStencilState;
		pipelineCI.pDynamicState = &dynamicState;
		pipelineCI.stageCount = static_cast<uint32_t>(2);
		pipelineCI.pStages = shaderStages.data();
		pipelineCI.pVertexInputState = vkglTF::Vertex::getPipelineVertexInputState({vkglTF::VertexComponent::Position, vkglTF::VertexComponent::Normal, vkglTF::VertexComponent::Color});
		;
		shaderStages[0] = loadShader(getShadersPath() + "imgui/scene.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
		shaderStages[1] = loadShader(getShadersPath() + "imgui/scene.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
		VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCI, nullptr, &pipelines.statue));

		// Node pipeline

		pipelineCI.layout = pipelineLayouts.node;

		// Vertex input bindings
		// The instancing pipeline uses a vertex input state with two bindings
		std::vector<VkVertexInputBindingDescription> bindingDescriptions = {
			// Binding point 0: Mesh vertex layout description at per-vertex rate
			vks::initializers::vertexInputBindingDescription(NODE_VERTEX_BIND_ID, sizeof(vkglTF::Vertex), VK_VERTEX_INPUT_RATE_VERTEX),
			// Binding point 1: Instanced data at per-instance rate
			vks::initializers::vertexInputBindingDescription(NODE_INSTANCE_BIND_ID, sizeof(NodeInstanceData), VK_VERTEX_INPUT_RATE_INSTANCE)};

		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
			vks::initializers::vertexInputAttributeDescription(NODE_VERTEX_BIND_ID, 0, VK_FORMAT_R32G32B32_SFLOAT, 0),				   // Location 0: Position
			vks::initializers::vertexInputAttributeDescription(NODE_VERTEX_BIND_ID, 1, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float) * 3), // Location 1: Normal
			vks::initializers::vertexInputAttributeDescription(NODE_VERTEX_BIND_ID, 2, VK_FORMAT_R32G32_SFLOAT, sizeof(float) * 6),	   // Location 2: Texture coordinates
			// Per-Instance attributes

			vks::initializers::vertexInputAttributeDescription(NODE_INSTANCE_BIND_ID, 3, VK_FORMAT_R32G32B32_SFLOAT, 0),				// Position
			vks::initializers::vertexInputAttributeDescription(NODE_INSTANCE_BIND_ID, 4, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float) * 3) // Rotation
		};

		VkPipelineVertexInputStateCreateInfo inputState = vks::initializers::pipelineVertexInputStateCreateInfo();
		inputState.pVertexBindingDescriptions = bindingDescriptions.data();
		inputState.pVertexAttributeDescriptions = attributeDescriptions.data();

		inputState.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
		inputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		pipelineCI.pVertexInputState = &inputState;
		pipelineCI.layout = pipelineLayouts.node;

		shaderStages[0] = loadShader(getShadersPath() + "imgui/node.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
		shaderStages[1] = loadShader(getShadersPath() + "imgui/node.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

		VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCI, nullptr, &pipelines.node));


	}

	void prepareInstanceData()
	{
		std::vector<NodeInstanceData> nodeInstanceData;
		nodeInstanceData.resize(NODE_INSTANCE_COUNT);

		std::random_device rnd;

		double xMin, xMax, yMin, yMax, zMin, zMax;
		xMin = -100.;
		xMax = 100.;
		yMin = -100.;
		yMax = 100.;
		zMin = -100.;
		zMax = 100.;

		std::uniform_real_distribution<float> distX(xMin, xMax);
		std::uniform_real_distribution<float> distY(yMin, yMax);
		std::uniform_real_distribution<float> distZ(zMin, zMax);

		for (uint32_t i = 0; i < NODE_INSTANCE_COUNT; i++)
		{
			nodeInstanceData[i].pos = {distX(rnd), distY(rnd), distZ(rnd)};
			nodeInstanceData[i].rot = {M_PI_2, .0f, .0f};
		}

		nodeInstanceBuffer.size = NODE_INSTANCE_COUNT * sizeof(NodeInstanceData);

		struct
		{
			VkDeviceMemory memory;
			VkBuffer buffer;
		} stagingBuffer;

		VK_CHECK_RESULT(vulkanDevice->createBuffer(
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			nodeInstanceBuffer.size,
			&stagingBuffer.buffer,
			&stagingBuffer.memory,
			nodeInstanceData.data()));

		VK_CHECK_RESULT(vulkanDevice->createBuffer(
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			nodeInstanceBuffer.size,
			&nodeInstanceBuffer.buffer,
			&nodeInstanceBuffer.memory));

		VkCommandBuffer copyCmd = vulkanDevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

		VkBufferCopy copyRegion = {};
		copyRegion.size = nodeInstanceBuffer.size;
		vkCmdCopyBuffer(
			copyCmd,
			stagingBuffer.buffer,
			nodeInstanceBuffer.buffer,
			1,
			&copyRegion);

		vulkanDevice->flushCommandBuffer(copyCmd, queue, true);

		nodeInstanceBuffer.descriptor.range = nodeInstanceBuffer.size;
		nodeInstanceBuffer.descriptor.buffer = nodeInstanceBuffer.buffer;
		nodeInstanceBuffer.descriptor.offset = 0;

		// Destroy staging resources
		vkDestroyBuffer(device, stagingBuffer.buffer, nullptr);
		vkFreeMemory(device, stagingBuffer.memory, nullptr);
	}

	// Prepare and initialize uniform buffer containing shader uniforms
	void prepareUniformBuffers()
	{
		// Vertex shader uniform buffer block
		VK_CHECK_RESULT(vulkanDevice->createBuffer(
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&uniformBuffers.VS,
			sizeof(uboVS),
			&uboVS));

		// Vertex shader uniform buffer block
		VK_CHECK_RESULT(vulkanDevice->createBuffer(
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&uniformBuffers.bezier,
			sizeof(uboBezier),
			&uboBezier));

		updateUniformBuffers();
	}

	std::chrono::time_point<std::chrono::steady_clock> t0 = std::chrono::steady_clock::now();

	void updateUniformBuffers()
	{
		// Vertex shader
		uboVS.projection = camera.matrices.perspective;
		uboVS.modelview = camera.matrices.view * glm::mat4(1.0f);

		float t_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count();

		// Light source
		if (uiSettings.animateLight)
		{
			uiSettings.lightTimer += frameTimer * uiSettings.lightSpeed;
			uboVS.lightPos.x = sin(glm::radians(uiSettings.lightTimer * 360.0f)) * 15.0f;
			uboVS.lightPos.z = cos(glm::radians(uiSettings.lightTimer * 360.0f)) * 15.0f;
		};
		uboVS.nodeRot = t_ms * M_PI_2 / 10000;

		VK_CHECK_RESULT(uniformBuffers.VS.map());
		memcpy(uniformBuffers.VS.mapped, &uboVS, sizeof(uboVS));
		uniformBuffers.VS.unmap();

		// Bezier geometry shader control points

		uboBezier.P1scale = glm::vec3(.25, .30, .0);
		uboBezier.P2scale = glm::vec3(.25, .30, .0);

		VK_CHECK_RESULT(uniformBuffers.bezier.map());
		memcpy(uniformBuffers.bezier.mapped, &uboBezier, sizeof(uboBezier));
		uniformBuffers.bezier.unmap();
	}

	void draw()
	{
		VulkanExampleBase::prepareFrame();
		buildCommandBuffers();
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &drawCmdBuffers[currentBuffer];
		VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
		VulkanExampleBase::submitFrame();
	}

	void loadAssets()
	{
		const uint32_t glTFLoadingFlags = vkglTF::FileLoadingFlags::PreTransformVertices | vkglTF::FileLoadingFlags::PreMultiplyVertexColors | vkglTF::FileLoadingFlags::FlipY;
		models.models.loadFromFile(getAssetPath() + "models/vulkanscenemodels.gltf", vulkanDevice, queue, glTFLoadingFlags);
		models.background.loadFromFile(getAssetPath() + "models/vulkanscenebackground.gltf", vulkanDevice, queue, glTFLoadingFlags);
		models.logos.loadFromFile(getAssetPath() + "models/vulkanscenelogos.gltf", vulkanDevice, queue, glTFLoadingFlags);
		models.node.loadFromFile(getAssetPath() + "models/ico_node.gltf", vulkanDevice, queue, glTFLoadingFlags);
	}

	void prepareImGui()
	{
		imGui = new ImGUI(this);
		imGui->init((float)width, (float)height);
		imGui->initResources(renderPass, queue, getShadersPath());
	}

	void prepare()
	{
		VulkanExampleBase::prepare();
		loadAssets();
		prepareUniformBuffers();
		prepareInstanceData();
		setupDescriptorSetLayouts();
		preparePipelines();
		setupDescriptorPool();
		setupDescriptorSets();
		prepareImGui();
		buildCommandBuffers();
		prepared = true;
	}

	virtual void render()
	{
		if (!prepared)
			return;

		// Update imGui
		ImGuiIO &io = ImGui::GetIO();

		io.DisplaySize = ImVec2((float)width, (float)height);
		io.DeltaTime = frameTimer;

		io.MousePos = ImVec2(mousePos.x, mousePos.y);
		io.MouseDown[0] = mouseButtons.left;
		io.MouseDown[1] = mouseButtons.right;

		draw();

		if (uiSettings.animateLight)
			updateUniformBuffers();
	}

	virtual void viewChanged()
	{
		updateUniformBuffers();
	}

	virtual void mouseMoved(double x, double y, bool &handled)
	{
		ImGuiIO &io = ImGui::GetIO();
		handled = io.WantCaptureMouse;
	}
};

VULKAN_EXAMPLE_MAIN()
