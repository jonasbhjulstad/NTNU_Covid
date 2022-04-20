/*
 * Vulkan |- imGui (https://github.com/ocornut/imgui)
 *
 * Copyright (C) 2017 by Sascha Willems - www.saschawillems.de
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#define KTX_OPENGL_ES3 1

#include <imgui.h>
#include "vulkanexamplebase.h"
#include "VulkanglTFModel.h"
#include <random>
#include <chrono>
#define ENABLE_VALIDATION true

#include "ImGuiApp.hpp"
#include "NV_glTFBasicInstance.hpp"
#include "NV_Node.hpp"
#include "NV_Bezier.hpp"
#include <igraph/igraph.h>
#include <igraph/igraph_games.h>
#include <igraph/igraph_layout.h>




// ----------------------------------------------------------------------------
// VulkanExample
// ----------------------------------------------------------------------------
#define NODE_VERTEX_BIND_ID 0
#define NODE_INSTANCE_BIND_ID 1
#define NODE_INSTANCE_COUNT 4
class VulkanExample : public VulkanExampleBase
{

#ifdef WIN32
	const std::string assetPath = "C:\\Users\\jonas\\Documents\\Vulkan\\examples\\imgui\\data\\";
	const std::string shadersPath = assetPath + "shaders\\";
	const std::string modelPath = assetPath + "models\\";
	const std::string texturePath = assetPath + "textures\\";
	const std::string fontPath = assetPath + "fonts\\DroidSansMono.ttf";
#else
	const std::string assetPath = "/home/deb/Documents/Vulkan/examples/imgui/data/";
	const std::string shadersPath = assetPath + "shaders/";
	const std::string modelPath = assetPath + "models/";
	const std::string texturePath = assetPath + "textures/";
	const std::string fontPath = assetPath + "fonts/DroidSansMono.ttf";
#endif

public:

	UISettings uiSettings;
	ImGUI *imGui = nullptr;

	struct
	{
		std::unique_ptr<BasicInstancePipelineData> node;
		std::unique_ptr<BasicInstancePipelineData> bezier;
	} graphicsPipelines;

	vks::Buffer uniformBufferVS;
	struct UBOVS {
		glm::mat4 projection;
		glm::mat4 modelview;
		glm::vec4 lightPos;
		float theta;
	} uboVS;

	struct {
		std::vector<NodeInstanceData> node;
		std::vector<BezierInstanceData> bezier;
	} instanceData;

	const uint32_t glTFLoadingFlags = vkglTF::FileLoadingFlags::PreTransformVertices | vkglTF::FileLoadingFlags::PreMultiplyVertexColors | vkglTF::FileLoadingFlags::FlipY;


	VulkanExample() : VulkanExampleBase(ENABLE_VALIDATION)
	{
		title = "Vulkan Example - ImGui";
		camera.type = Camera::CameraType::firstperson;
		camera.setPosition(glm::vec3(0.0f, 0.0f, -4.8f));
		camera.setRotation(glm::vec3(4.5f, 380.0f, 0.0f));
		camera.setPerspective(45.0f, (float)width / (float)height, 0.1f, 256.0f);
		camera.movementSpeed = 20.0f;
		// Don't use the ImGui overlay of the base framework in this sample
		settings.overlay = false;
		uint32_t propCount;
		width = 2736;
		height = 1824; 

	}

	void keyPressed(uint32_t key)
	{

		// switch(key)
		// {
		// 	case :
		// 	break;

		// 	case KEY_S:
		// 	break;

		// 	case KEY_D:
		// 	break;

		// 	case KEY_W:
				
		// 	break;
		// }
		std::cout << "key:\t" << key << std::endl;
	}

	~VulkanExample()
	{
		delete imGui;
	}

	void buildCommandBuffers()
	{
		VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

		VkClearValue clearValues[2];
		clearValues[0].color = {{162./255., 157./255., 160./255., 0.5}};
		clearValues[1].depthStencil = {1.0f, 0};

		VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
		renderPassBeginInfo.renderPass = renderPass;
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = width;
		renderPassBeginInfo.renderArea.extent.height = height;
		renderPassBeginInfo.clearValueCount = 2;
		renderPassBeginInfo.pClearValues = clearValues;

		imGui->newFrame(this, (frameCounter == 0), uiSettings);

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

			VkDeviceSize offset = 0;

			graphicsPipelines.node->offset = &offset;
			if (uiSettings.displayNodes)
			{
				buildCommandBuffer(*graphicsPipelines.node, drawCmdBuffers[i]);
			}
			graphicsPipelines.bezier->offset = &offset;
			if (uiSettings.displayEdges)
			{
				buildCommandBuffer(*graphicsPipelines.bezier, drawCmdBuffers[i]);
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

		VkDescriptorPoolCreateInfo descriptorPoolInfo = vks::initializers::descriptorPoolCreateInfo(poolSizes, 2);
		VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &descriptorPool));
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
		uiSettings.fontPath = fontPath;
		imGui->init((float)width, (float)height, uiSettings);
		imGui->initResources(renderPass, queue, shadersPath);
	}

	void prepareNodeInstanceData()
	{

		instanceData.node.reserve(NODE_INSTANCE_COUNT);
		igraph_t graph;
		igraph_vector_t component_sizes;
		igraph_rng_seed(igraph_rng_default(), 42);

		int N_nodes = NODE_INSTANCE_COUNT;
		double ER_p = .7;
		igraph_erdos_renyi_game(&graph, IGRAPH_ERDOS_RENYI_GNP, N_nodes, ER_p, IGRAPH_UNDIRECTED, IGRAPH_NO_LOOPS);
		igraph_vector_t minB, maxB;
		igraph_vector_init(&minB, N_nodes);
		igraph_vector_init(&maxB, N_nodes);


		long int width = 100;
		long int height = 100;
		igraph_matrix_t mat;
		igraph_matrix_init(&mat, N_nodes, 3);
		igraph_vector_init(&minB, N_nodes);
		igraph_vector_init(&maxB, N_nodes);
		igraph_vector_fill(&minB, -100.);
		igraph_vector_fill(&maxB, 100.);
		igraph_layout_grid_3d(&graph, &mat, 0, 0);
		igraph_matrix_scale(&mat, 50);
		// igraph_layout_kamada_kawai_3d(&graph, &mat, 1, 10*N_nodes, 0, igraph_ecount(&graph), nullptr, &minB, &maxB, &minB, &maxB, &minB, &maxB);
		// igraph_layout_fruchterman_reingold_3d(&graph, &mat, 1, 500, 10000., nullptr, &minB, &maxB, &minB, &maxB, &minB, &maxB);
		std::cout << "Matrix size: " << igraph_matrix_nrow(&mat) << "x" << igraph_matrix_ncol(&mat) << std::endl;
		// igraph_layout_grid_3d(&graph, &mat, width, height);
		std::uniform_real_distribution<float> distScale(.2f, 2.0f);
		for (int i = 0; i < NODE_INSTANCE_COUNT; i++)
		{
			std::cout << "Position: " << igraph_matrix_e(&mat, i, 0) << " " << igraph_matrix_e(&mat, i, 1) << " " << igraph_matrix_e(&mat, i, 2) << std::endl;
			instanceData.node.push_back({glm::vec3(igraph_matrix_e(&mat, i, 0), igraph_matrix_e(&mat, i, 1), igraph_matrix_e(&mat, i, 2)), 1.0f});
		}

	}

	void prepareBezierInstanceData(std::vector<NodeInstanceData> &nodeInstanceData)
	{
		const uint32_t N_nodes = static_cast<uint32_t>(nodeInstanceData.size());

		instanceData.bezier.reserve(N_nodes * N_nodes);
		for (uint32_t i = 0; i < N_nodes; i++)
		{
			for (uint32_t j = i+1; j < N_nodes; j++)
			{
				instanceData.bezier.push_back({nodeInstanceData[i].pos, nodeInstanceData[j].pos, {1.0f,1.0f,1.0f}});
			}
		}
	}

	void prepareNode()
	{
		prepareNodeInstanceData();

		BasicInstancedRenderingParams nodeParam;

		nodeParam.vertexShaderPath = shadersPath + "node.vert.spv";
		nodeParam.fragmentShaderPath = shadersPath + "node.frag.spv";
		nodeParam.modelPath = modelPath + "ico_node.gltf";
		nodeParam.texturePath = texturePath + "icoTex2.ktx";

		nodeParam.vulkanDevice = vulkanDevice;
		nodeParam.uniformProjectionBuffer = &uniformBufferVS;
		nodeParam.queue = queue;
		nodeParam.descriptorPool = descriptorPool;
		nodeParam.pipelineCache = pipelineCache;
		nodeParam.renderPass = renderPass;

		graphicsPipelines.node = prepareBasicInstancedRendering(instanceData.node, nodeParam);

	}


	void prepareBezier()
	{
		prepareBezierInstanceData(instanceData.node);


		BasicInstancedRenderingParams bezierParam;

		bezierParam.vertexShaderPath = shadersPath + "bezier.vert.spv";
		bezierParam.fragmentShaderPath = shadersPath + "bezier.frag.spv";
		bezierParam.modelPath = modelPath + "bezier.gltf";

		bezierParam.vulkanDevice = vulkanDevice;
		bezierParam.uniformProjectionBuffer = &uniformBufferVS;
		bezierParam.queue = queue;
		bezierParam.descriptorPool = descriptorPool;
		bezierParam.pipelineCache = pipelineCache;
		bezierParam.renderPass = renderPass;

		graphicsPipelines.bezier = prepareBasicInstancedRendering(instanceData.bezier, bezierParam);

	}

	void prepare()
	{

		VulkanExampleBase::prepare();
		prepareUniformBuffers();
		setupDescriptorPool();
		prepareImGui();

		prepareNode();
		prepareBezier();

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

		uiSettings.popup = uiSettings.popup || io.MouseDown[1];

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

	void updateUniformBuffers()
	{
		uint32_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()%3600;
		// Vertex shader
		uboVS.projection = camera.matrices.perspective;
		uboVS.modelview = camera.matrices.view * glm::mat4(1.0f);
		uboVS.theta = (float)(time)/3600.*2*M_PI;
		// Light source
		if (uiSettings.animateLight) {
			uiSettings.lightTimer += frameTimer * uiSettings.lightSpeed;
			uboVS.lightPos.x = sin(glm::radians(uiSettings.lightTimer * 360.0f)) * 15.0f;
			uboVS.lightPos.z = cos(glm::radians(uiSettings.lightTimer * 360.0f)) * 15.0f;
		};

		VK_CHECK_RESULT(uniformBufferVS.map());
		memcpy(uniformBufferVS.mapped, &uboVS, sizeof(uboVS));
		uniformBufferVS.unmap();
	}

};

VULKAN_EXAMPLE_MAIN()
