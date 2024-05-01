#define KTX_OPENGL_ES3 1
#define ENABLE_VALIDATION true

// #include <VulkanViewport/VulkanglTFModel.h>
#include <GLFW/glfw3.h>
#include <VulkanTools/Camera.hpp>
#include <VulkanTools/ProjectionBuffer.hpp>
#include <VulkanTools/Setup.hpp>
#include <VulkanTools/Window.hpp>
#include <VulkanTools/gltf/glTFModel.hpp>
#include <VulkanViewport/Filepaths.hpp>
#include <VulkanViewport/Graph_Layout.hpp>
#include <VulkanViewport/ImGuiUI.hpp>
#include <VulkanViewport/Instance/Voxel.hpp>
#include <VulkanViewport/SetupRoutines.hpp>
#include <VulkanViewport/UISettings.hpp>
#include <chrono>
#include <imgui/imgui.h>
#include <memory>
#include <random>
#include <vulkan/vulkan.hpp>

#include <random>

std::vector<VkVP::VoxelInstanceData> generate_ground(uint32_t Nx, uint32_t Ny,
                                               uint32_t Nz) {
  std::vector<VkVP::VoxelInstanceData> ground(Nx * Ny * Nz);

  for (uint32_t i = 0; i < Nx; i++) {
    for (uint32_t j = 0; j < Ny; j++) {
      for (uint32_t k = 0; k < Nz; k++) {
        ground[i * Ny * Nz + j * Nz + k].pos = glm::vec3(i, j, k);
        ground[i * Ny * Nz + j * Nz + k].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
      }
    }
  }
  return ground;
}

void setupDescriptorPool(VkDevice logicalDevice,
                         VkDescriptorPool &descriptorPool) {
  // Example uses one ubo
  std::vector<VkDescriptorPoolSize> poolSizes = {
      initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2),
      initializers::descriptorPoolSize(
          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2),
  };

  VkDescriptorPoolCreateInfo descriptorPoolInfo =
      initializers::descriptorPoolCreateInfo(poolSizes.size(), poolSizes.data(),
                                             2);

  VK_CHECK_RESULT(vkCreateDescriptorPool(logicalDevice, &descriptorPoolInfo,
                                         nullptr, &descriptorPool));
}

int main() {
  using namespace VkVP;
  VulkanInstance vulkanInstance;

  /* Setup, Integration and Initialization of Vulkan/GLFW */

  if (!glfwInit())
    return 1;

  int width = 1280;
  int height = 720;

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  vulkanInstance.glfwWindow =
      glfwCreateWindow(width, height, "VoxelGround", NULL, NULL);

  if (!glfwVulkanSupported()) {
    printf("GLFW: Vulkan Not Supported\n");
    return 1;
  }
  uint32_t extensions_count = 0;
  const char **extensions =
      glfwGetRequiredInstanceExtensions(&extensions_count);
  for (int i = 0; i < extensions_count; i++) {
    vulkanInstance.enabledInstanceExtensions.push_back(extensions[i]);
  }

  createVulkanInstance(ENABLE_VALIDATION, "VoxelGround", vulkanInstance.instance,
                       vulkanInstance.supportedInstanceExtensions,
                       vulkanInstance.enabledInstanceExtensions,
                       VK_API_VERSION_1_0);
  setupVulkanPhysicalDevice(vulkanInstance, ENABLE_VALIDATION);

  VK_CHECK_RESULT(glfwCreateWindowSurface(vulkanInstance.instance,
                                          vulkanInstance.glfwWindow, NULL,
                                          &vulkanInstance.surface));

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

  using namespace VkVP;

  prepareProjectionBuffer(vulkanDevice, vulkanInstance.projection.buffer,
                          vulkanInstance.projection.data, camera);

  VkDescriptorPool renderDescriptorPool;
  setupDescriptorPool(vulkanDevice->logicalDevice, renderDescriptorPool);

  VkDeviceSize offset[1] = {0};
  using namespace glTFBasicInstance;

  InstanceRenderingParams voxelParams;
  voxelParams.vertexShaderPath = SHADER_DIR + "cube.vert.spv";
  voxelParams.fragmentShaderPath = SHADER_DIR + "cube.frag.spv";
  voxelParams.modelPath = MODEL_DIR + "cube.glb";
  voxelParams.vulkanDevice = vulkanInstance.vulkanDevice;
  voxelParams.uniformProjectionBuffer = &vulkanInstance.projection.buffer;
  voxelParams.queue = vulkanInstance.queue;
  voxelParams.renderPass = vulkanInstance.renderPass;
  voxelParams.pipelineCache = vulkanInstance.pipelineCache;
  voxelParams.descriptorPool = renderDescriptorPool;
  voxelParams.offset = offset;

  auto voxelInstanceData = generate_ground(10, 10, 10);

  std::vector<std::unique_ptr<InstancePipelineData>> instancePipelines;
  instancePipelines.push_back(prepareInstanceRendering<VoxelInstanceData>(
      voxelParams, voxelInstanceData));
  // instancePipelines.push_back(prepareInstanceRendering<EdgeInstanceData>(edgeParams,
  // edgeInstanceData));

  /* ImGUI App Initialization */

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiContext *g = ImGui::GetCurrentContext();
  camera.setContext(g);
  ImGuiIO &io = ImGui::GetIO();
  camera.mousePos_old = {io.MousePos.x, io.MousePos.y};
  ImGui_Vulkan_Init(vulkanInstance);

  // camera.setWindowID(ImGui::GetCurrentWindow());

  VkVP::ImGuiVulkanData ivData(vulkanInstance.vulkanDevice);

  VkVP::setupImGuiVisuals(width, height, uiSettings);

  VkVP::initializeImGuiVulkanResources(ivData, vulkanInstance.renderPass,
                                       vulkanInstance.queue,
                                       ASSET_DIR + "shaders/");

  /* Render-loop variables */
  bool rebuildSwapChain = false;
  uint32_t currentBufferIdx;
  float frameTimer;
  auto tStart = std::chrono::high_resolution_clock::now();

  while (!glfwWindowShouldClose(vulkanInstance.glfwWindow)) {
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    camera.update(frameTimer);
    auto tEnd = std::chrono::high_resolution_clock::now();
    auto tDiff =
        std::chrono::duration<double, std::milli>(tEnd - tStart).count();
    frameTimer = (float)tDiff / 1000.0f;
    tStart = tEnd;

    igraph_t newGraph;
    VkVP::newFrame(uiSettings, frameTimer, camera, &newGraph);

    VkVP::updateBuffers(vulkanInstance.vulkanDevice, ivData.vertexBuffer,
                        ivData.indexBuffer, ivData.indexCount,
                        ivData.vertexCount);

    updateProjectionBuffer(vulkanInstance.projection.buffer,
                           vulkanInstance.projection.data, camera, true);

    updateWindowSize(vulkanInstance, ivData, camera, instancePipelines, width,
                     height);

    buildCommandBuffers(vulkanInstance.drawCmdBuffers,
                        vulkanInstance.frameBuffers, vulkanInstance.renderPass,
                        ivData, instancePipelines, width, height);

    submitBuffers(vulkanInstance, currentBufferIdx);
  }

  ImGui_ImplVulkanH_DestroyWindow(vulkanInstance.instance,
                                  vulkanDevice->logicalDevice,
                                  &vulkanInstance.ImGuiWindow, NULL);
  vkDestroyDescriptorPool(vulkanDevice->logicalDevice,
                          vulkanInstance.descriptorPool, NULL);

#ifdef IMGUI_VULKAN_DEBUG_REPORT
  // Remove the debug report callback
  auto vkDestroyDebugReportCallbackEXT =
      (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(
          g_Instance, "vkDestroyDebugReportCallbackEXT");
  vkDestroyDebugReportCallbackEXT(g_Instance, g_DebugReport, g_Allocator);
#endif // IMGUI_VULKAN_DEBUG_REPORT

  glfwDestroyWindow(vulkanInstance.glfwWindow);
  glfwTerminate();

  return 0;
}