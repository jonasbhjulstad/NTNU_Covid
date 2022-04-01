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
#include "gltfBasicInstance.hpp"
#include "computeBasic.hpp"
#include "node.hpp"
#include "bezier.hpp"
// ----------------------------------------------------------------------------
// VulkanExample
// ----------------------------------------------------------------------------
#define NODE_VERTEX_BIND_ID 0
#define NODE_INSTANCE_BIND_ID 1
#define NODE_INSTANCE_COUNT 3
class VulkanExample : public VulkanExampleBase
{

#ifdef WIN32
	const std::string assetPath = "C:\\Users\\jonas\\Documents\\Vulkan\\examples\\imgui\\data\\";
	const std::string shadersPath = assetPath + "shaders\\";
	const std::string modelPath = assetPath + "models\\";
	const std::string texturePath = assetPath + "textures\\";
#else
	const std::string assetPath = "/home/deb/Documents/Vulkan/examples/imgui/data/";
	const std::string shadersPath = assetPath + "shaders/";
	const std::string modelPath = assetPath + "models/";
	const std::string texturePath = assetPath + "textures/";
#endif

public:
	ImGUI *imGui = nullptr;
	const uint32_t N_instance_models = 2;
	struct Models
	{
		vkglTF::Model node;
		vkglTF::Model bezier;
	} models;

	struct Textures
	{
		vks::Texture2DArray node;
		vks::Texture2DArray bezier;
	} textures;

	struct BasicInstanceModels
	{
		glTFBasicInstance<NodeInstanceData> *node;
		glTFBasicInstance<BezierInstanceData> *bezier;
	} BIModels;

	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	std::string modelAssetPath;

	vks::Buffer uniformBufferVS;

	struct UBOVS
	{
		glm::mat4 projection;
		glm::mat4 modelview;
		glm::vec4 lightPos;
		float theta;
	} uboVS;

	VulkanExample() : VulkanExampleBase(ENABLE_VALIDATION)
	{
		title = "Vulkan Example - ImGui";
		camera.type = Camera::CameraType::lookat;
		camera.setPosition(glm::vec3(0.0f, 0.0f, -4.8f));
		camera.setRotation(glm::vec3(4.5f, -380.0f, 0.0f));
		camera.setPerspective(45.0f, (float)width / (float)height, 0.1f, 256.0f);
		// Don't use the ImGui overlay of the base framework in this sample
		settings.overlay = false;
	}

	~VulkanExample()
	{
		uniformBufferVS.destroy();
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

			VkDeviceSize offsets[1] = {0};

			if (uiSettings.displayNodes)
			{
				BIModels.node->buildCommandBuffer(drawCmdBuffers[i], offsets);
			}
			if (uiSettings.displayEdges)
			{
				BIModels.bezier->buildCommandBuffer(drawCmdBuffers[i], offsets);
			}

			// Render imGui
			imGui->drawFrame(drawCmdBuffers[i]);

			vkCmdEndRenderPass(drawCmdBuffers[i]);

			VK_CHECK_RESULT(vkEndCommandBuffer(drawCmdBuffers[i]));
		}
	}

	void setupDescriptorPool()
	{
		const uint32_t N_GLTF_BASIC_INSTANCE_TYPES = 2;
		std::vector<VkDescriptorPoolSize> poolSizes =
			{
				vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, N_GLTF_BASIC_INSTANCE_TYPES),
				vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, N_GLTF_BASIC_INSTANCE_TYPES)};

		auto computePoolSizes = ComputeBasic::getPoolSizes();
		poolSizes.insert(poolSizes.end(), computePoolSizes.begin(), computePoolSizes.end());

		VkDescriptorPoolCreateInfo descriptorPoolInfo = vks::initializers::descriptorPoolCreateInfo(poolSizes, 2);
		VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &descriptorPool));
	}

	// Prepare and initialize uniform buffer containing shader uniforms
	void prepareUniformBuffers()
	{
		// Vertex shader uniform buffer block
		VK_CHECK_RESULT(vulkanDevice->createBuffer(
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&uniformBufferVS,
			sizeof(uboVS),
			&uboVS));

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

		uboVS.theta = t_ms/1000*M_PI_2;


		VK_CHECK_RESULT(uniformBufferVS.map());
		memcpy(uniformBufferVS.mapped, &uboVS, sizeof(uboVS));
		uniformBufferVS.unmap();
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
	void prepareImGui()
	{
		imGui = new ImGUI(this);
		imGui->init((float)width, (float)height);
		imGui->initResources(renderPass, queue, shadersPath);
	}

	std::vector<NodeInstanceData> prepareNodeInstanceData()
	{
		std::vector<NodeInstanceData> instanceData(NODE_INSTANCE_COUNT);

		float limits[] = {-10., 10., -10., 10., -10., 10.};
		std::random_device rnd;
		std::uniform_real_distribution<float> distX(limits[0], limits[1]);
		std::uniform_real_distribution<float> distY(limits[2], limits[3]);
		std::uniform_real_distribution<float> distZ(limits[4], limits[5]);
		std::uniform_real_distribution<float> distScale(.2f, 2.0f);
		for (int i = 0; i < NODE_INSTANCE_COUNT; i++)
		{
			instanceData[i].pos = glm::vec3(distX(rnd), distY(rnd), distZ(rnd));
			instanceData[i].scale = distScale(rnd);
		}

		return instanceData;
	}

	std::vector<BezierInstanceData> prepareBezierInstanceData(std::vector<NodeInstanceData> &nodeInstanceData)
	{
		const uint32_t N_nodes = nodeInstanceData.size();
		std::vector<BezierInstanceData> instanceData;
		instanceData.reserve(N_nodes * N_nodes);
		for (int i = 0; i < N_nodes; i++)
		{
			for (int j = i+1; j < N_nodes; j++)
			{
				instanceData.push_back({nodeInstanceData[i].pos, nodeInstanceData[j].pos});
			}
		}
		return instanceData;
	}

	void prepare()
	{
		VulkanExampleBase::prepare();
		prepareUniformBuffers();
		setupDescriptorPool();

		prepareImGui();

		const uint32_t glTFLoadingFlags = vkglTF::FileLoadingFlags::PreTransformVertices | vkglTF::FileLoadingFlags::PreMultiplyVertexColors | vkglTF::FileLoadingFlags::FlipY;
		models.node.loadFromFile(modelPath + "ico_node.gltf", vulkanDevice, queue, glTFLoadingFlags);
		textures.node.loadFromFile(texturePath + "icoTex2.ktx", VK_FORMAT_R8G8B8A8_UNORM, vulkanDevice, queue);
		models.bezier.loadFromFile(modelPath + "bezier.gltf", vulkanDevice, queue, glTFLoadingFlags);
		// textures.bezier.loadFromFile(texturePath + "bezier.ktx", VK_FORMAT_R8G8B8A8_UNORM, vulkanDevice, queue);
		BIModels.node = new glTFBasicInstance<NodeInstanceData>(device, vulkanDevice, &uniformBufferVS, queue,
																shadersPath + "node.vert.spv", shadersPath + "node.frag.spv", modelPath + "ico_node.gltf", renderPass, &models.node, &textures.node);

		auto nodeInstanceData = prepareNodeInstanceData();
		BIModels.node->prepare(nodeInstanceData, descriptorPool);

		BIModels.bezier = new glTFBasicInstance<BezierInstanceData>(device, vulkanDevice, &uniformBufferVS, queue,
																	shadersPath + "bezier.vert.spv", shadersPath + "bezier.frag.spv", modelPath + "bezier.gltf", renderPass, &models.bezier);

		BIModels.bezier->prepare(prepareBezierInstanceData(nodeInstanceData), descriptorPool);

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
