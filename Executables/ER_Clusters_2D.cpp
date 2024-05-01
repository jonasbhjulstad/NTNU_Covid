#define KTX_OPENGL_ES3 1
#define ENABLE_VALIDATION true

// #include <VulkanViewport/VulkanglTFModel.h>
#include <random>
#include <chrono>
#include <memory>
#include <vulkan/vulkan.hpp>
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>
#include <VulkanTools/Setup.hpp>
#include <VulkanTools/Window.hpp>
#include <VulkanTools/Camera.hpp>
#include <VulkanTools/ProjectionBuffer.hpp>
#include <VulkanViewport/ImGuiUI.hpp>
#include <VulkanViewport/Graph_Layout.hpp>
#include <VulkanTools/gltf/glTFModel.hpp>
#include <VulkanViewport/UISettings.hpp>
#include <VulkanViewport/SetupRoutines.hpp>
#include <random>
#include <VulkanViewport/Filepaths.hpp>


void setupDescriptorPool(VkDevice logicalDevice, VkDescriptorPool& descriptorPool)
{
    // Example uses one ubo
    std::vector<VkDescriptorPoolSize> poolSizes =
    {
        initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2),
        initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2),
    };

    VkDescriptorPoolCreateInfo descriptorPoolInfo =
        initializers::descriptorPoolCreateInfo(
            poolSizes.size(),
            poolSizes.data(),
            2);

    VK_CHECK_RESULT(vkCreateDescriptorPool(logicalDevice, &descriptorPoolInfo, nullptr, &descriptorPool));
}

int main()
{
    using namespace VkVP;
    VulkanInstance vulkanInstance;

    /* Setup, Integration and Initialization of Vulkan/GLFW */

    if (!glfwInit())
        return 1;

    int width = 1280;
    int height = 720;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    vulkanInstance.glfwWindow = glfwCreateWindow(width, height, "Network Viewport", NULL, NULL);

    if (!glfwVulkanSupported())
    {
        printf("GLFW: Vulkan Not Supported\n");
        return 1;
    }
    uint32_t extensions_count = 0;
    const char **extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
    for (int i = 0; i < extensions_count; i++)
    {
        vulkanInstance.enabledInstanceExtensions.push_back(extensions[i]);
    }

    createVulkanInstance(ENABLE_VALIDATION, "Network Viewport", vulkanInstance.instance, vulkanInstance.supportedInstanceExtensions, vulkanInstance.enabledInstanceExtensions, VK_API_VERSION_1_0);
    setupVulkanPhysicalDevice(vulkanInstance, ENABLE_VALIDATION);

    VK_CHECK_RESULT(glfwCreateWindowSurface(vulkanInstance.instance, vulkanInstance.glfwWindow, NULL, &vulkanInstance.surface));

    VulkanDevice *vulkanDevice = vulkanInstance.vulkanDevice;
    setupGLFWVulkanWindow(vulkanInstance, width, height, 2);
    prepareVulkan(vulkanInstance, width, height);

    /* User Interface Settings */

    UISettings uiSettings;
    uiSettings.fontPath = ASSET_DIR + "fonts/DroidSansMono.ttf";
    Camera camera;
    camera.type = camera.firstperson;
    camera.position = glm::vec3(0.0f, 0.0f, -10.0f);
    camera.rotation = glm::vec3(-45.0f, 0.0f, 0.0f);
    camera.setPerspective(60.0f, (float)width / (float)height, 0.1f, 1000.0f);


    size_t N_nodes = 100;
    float nodePosOffset[3] = {0.f, 0.f, 0.f};
    
    igraph_t graph;
    igraph_erdos_renyi_game(&graph, IGRAPH_ERDOS_RENYI_GNP, N_nodes, 0.5, 0, 0);
    using namespace VkVP;

    auto nodeInstanceData = kamada_kawai_2D(graph, 500, 0);
    auto edgeInstanceData = get_edge_positions(nodeInstanceData, graph);


    prepareProjectionBuffer(vulkanDevice, vulkanInstance.projection.buffer, vulkanInstance.projection.data, camera);


    VkDescriptorPool renderDescriptorPool;
    setupDescriptorPool(vulkanDevice->logicalDevice, renderDescriptorPool);

    VkDeviceSize offset[1] = {0};
    using namespace glTFBasicInstance;

    InstanceRenderingParams nodeParams;
    nodeParams.vertexShaderPath = SHADER_DIR + "node.vert.spv";
    nodeParams.fragmentShaderPath = SHADER_DIR + "node.frag.spv";
    nodeParams.modelPath = MODEL_DIR + "ico_node.gltf";
    nodeParams.vulkanDevice = vulkanInstance.vulkanDevice;
    nodeParams.uniformProjectionBuffer = &vulkanInstance.projection.buffer;
    nodeParams.queue = vulkanInstance.queue;
    nodeParams.renderPass = vulkanInstance.renderPass;
    nodeParams.pipelineCache = vulkanInstance.pipelineCache;
    nodeParams.descriptorPool = renderDescriptorPool;
    nodeParams.offset = offset;

    InstanceRenderingParams edgeParams;
    edgeParams.vertexShaderPath = SHADER_DIR + "edge.vert.spv";
    edgeParams.fragmentShaderPath = SHADER_DIR + "edge.frag.spv";
    edgeParams.modelPath = MODEL_DIR + "bezier.gltf";
    edgeParams.vulkanDevice = vulkanInstance.vulkanDevice;
    edgeParams.uniformProjectionBuffer = &vulkanInstance.projection.buffer;
    edgeParams.queue = vulkanInstance.queue;
    edgeParams.renderPass = vulkanInstance.renderPass;
    edgeParams.pipelineCache = vulkanInstance.pipelineCache;
    edgeParams.descriptorPool = renderDescriptorPool;
    edgeParams.offset = offset;
    std::vector<std::unique_ptr<InstancePipelineData>> instancePipelines;
    instancePipelines.push_back(prepareInstanceRendering<NodeInstanceData>(nodeParams, nodeInstanceData));
    instancePipelines.push_back(prepareInstanceRendering<EdgeInstanceData>(edgeParams, edgeInstanceData));    


    /* ImGUI App Initialization */

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiContext* g = ImGui::GetCurrentContext();
    camera.setContext(g);
    ImGuiIO& io = ImGui::GetIO();
    camera.mousePos_old = {io.MousePos.x, io.MousePos.y};
    ImGui_Vulkan_Init(vulkanInstance);

    // camera.setWindowID(ImGui::GetCurrentWindow());

    VkVP::ImGuiVulkanData ivData(vulkanInstance.vulkanDevice);

    VkVP::setupImGuiVisuals(width, height, uiSettings);

    VkVP::initializeImGuiVulkanResources(ivData, vulkanInstance.renderPass, vulkanInstance.queue, ASSET_DIR + "shaders/");




    /* Render-loop variables */
    bool rebuildSwapChain = false;
    uint32_t currentBufferIdx;
    float frameTimer;
    auto tStart = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(vulkanInstance.glfwWindow))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        camera.update(frameTimer);
        auto tEnd = std::chrono::high_resolution_clock::now();
        auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
        frameTimer = (float)tDiff / 1000.0f;
        tStart = tEnd;

        igraph_t newGraph;
        VkVP::newFrame(uiSettings, frameTimer, camera, &newGraph);

        VkVP::updateBuffers(vulkanInstance.vulkanDevice, ivData.vertexBuffer, ivData.indexBuffer, ivData.indexCount, ivData.vertexCount);

        updateProjectionBuffer(vulkanInstance.projection.buffer, vulkanInstance.projection.data, camera, true);

        updateWindowSize(vulkanInstance, ivData, camera, instancePipelines, width, height);

        buildCommandBuffers(vulkanInstance.drawCmdBuffers, vulkanInstance.frameBuffers, vulkanInstance.renderPass, ivData, instancePipelines, width, height);

        submitBuffers(vulkanInstance, currentBufferIdx);

    }

    ImGui_ImplVulkanH_DestroyWindow(vulkanInstance.instance, vulkanDevice->logicalDevice, &vulkanInstance.ImGuiWindow, NULL);
    vkDestroyDescriptorPool(vulkanDevice->logicalDevice, vulkanInstance.descriptorPool, NULL);

#ifdef IMGUI_VULKAN_DEBUG_REPORT
    // Remove the debug report callback
    auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkDestroyDebugReportCallbackEXT");
    vkDestroyDebugReportCallbackEXT(g_Instance, g_DebugReport, g_Allocator);
#endif // IMGUI_VULKAN_DEBUG_REPORT

    glfwDestroyWindow(vulkanInstance.glfwWindow);
    glfwTerminate();

    return 0;
}