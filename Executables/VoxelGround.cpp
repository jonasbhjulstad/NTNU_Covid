#define KTX_OPENGL_ES3 1
#define ENABLE_VALIDATION true
#define GLFW_INCLUDE_VULKAN
// #include <VulkanViewport/VulkanglTFModel.h>
#include <GLFW/glfw3.h>
#include <VulkanTools/Camera.hpp>
#include <VulkanTools/ProjectionBuffer.hpp>
#include <VulkanTools/Setup.hpp>
#include <VulkanTools/Window.hpp>
#include <VulkanTools/Renderpass.hpp>
#include <VulkanTools/gltf/glTFModel.hpp>
#include <VulkanViewport/Filepaths.hpp>
#include <VulkanViewport/Instance/Voxel.hpp>
#include <VulkanTools/InstanceRendering/InstancePipeline.hpp>
#include <VulkanViewport/UI/ImGuiUI.hpp>
#include <VulkanTools/Submit.hpp>
#include <memory>
#include <chrono>
#include <random>

std::vector<VkVP::VoxelInstanceData> generate_ground(uint32_t Nx, uint32_t Ny,
                                                     uint32_t Nz) {
  std::vector<VkVP::VoxelInstanceData> ground(Nx * Ny * Nz);

  for (uint32_t i = 0; i < Nx; i++) {
    for (uint32_t j = 0; j < Ny; j++) {
      for (uint32_t k = 0; k < Nz; k++) {
        ground[i * Ny * Nz + j * Nz + k].pos = glm::vec3(i, j, k);
        // ground[i * Ny * Nz + j * Nz + k].color = glm::vec4(1.0f, 0.0f,
        // 0.0f, 1.0f);
        ground[i * Ny * Nz + j * Nz + k].scale = glm::vec3(1.0f, 1.0f, 1.0f);
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
  descriptorPoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
  VK_CHECK_RESULT(vkCreateDescriptorPool(logicalDevice, &descriptorPoolInfo,
                                         nullptr, &descriptorPool));
}


int main() {
  using namespace VkVP;
  VulkanInstance vulkanInstance;

  /* Setup, Integration and Initialization of Vulkan/GLFW */

  if (!glfwInit())
    return 1;

  uint32_t width = 1280;
  uint32_t height = 720;
  VkVP::assignGLFWRequiredInstanceExtensions(vulkanInstance);
  vulkanInstance.glfwWindow = VkVP::createGLFWWindow(width, height, "VoxelGround");
  createVulkanInstance(
      ENABLE_VALIDATION, "VoxelGround", vulkanInstance.instance,
      vulkanInstance.supportedInstanceExtensions,
      vulkanInstance.enabledInstanceExtensions, VK_API_VERSION_1_0);
  setupVulkanPhysicalDevice(vulkanInstance, ENABLE_VALIDATION);
  VulkanDevice *vulkanDevice = vulkanInstance.vulkanDevice;
  VK_CHECK_RESULT(glfwCreateWindowSurface(vulkanInstance.instance, vulkanInstance.glfwWindow, nullptr, &vulkanInstance.surface));
  prepareVulkan(vulkanInstance, width, height);
  VkVP::setupGLFWVulkanWindow(vulkanInstance, width, height);
  // VkVP::setupGLFWVulkanWindow(vulkanInstance, width, height);
  /* User Interface Settings */
  std::string fontPath = ASSET_DIR + "fonts/DroidSansMono.ttf";
  Camera camera;
  camera.type = camera.firstperson;
  camera.position = glm::vec3(0.0f, 0.0f, -10.0f);
  camera.rotation = glm::vec3(-45.0f, 0.0f, 0.0f);
  camera.setPerspective(60.0f, (float)width / (float)height, 0.1f, 1000.0f);


  prepareProjectionBuffer(vulkanDevice, vulkanInstance.projection.buffer,
                          vulkanInstance.projection.data, camera);

  setupDescriptorPool(vulkanDevice->logicalDevice, vulkanInstance.descriptorPool);
  VkDeviceSize offset[1] = {0};
  auto voxelInstanceData = generate_ground(1, 1, 1);
  VkVP::InstanceRenderingParams voxelParams;
  voxelParams.vertexShaderPath = SHADER_DIR + "cube.vert.spv";
  voxelParams.fragmentShaderPath = SHADER_DIR + "cube.frag.spv";
  voxelParams.modelPath = MODEL_DIR + "monkey.gltf";
  voxelParams.vulkanDevice = vulkanInstance.vulkanDevice;
  voxelParams.uniformProjectionBuffer = &vulkanInstance.projection.buffer;
  voxelParams.queue = vulkanInstance.queue;
  voxelParams.renderPass = vulkanInstance.renderPass;
  voxelParams.pipelineCache = vulkanInstance.pipelineCache;
  voxelParams.descriptorPool = vulkanInstance.descriptorPool;
  voxelParams.offset = offset;

  std::vector<std::unique_ptr<VkVP::InstancePipelineData>> instancePipelines;
  instancePipelines.push_back(VkVP::prepareInstanceRendering<VoxelInstanceData>(
      voxelParams, voxelInstanceData));

  /* ImGUI App Initialization */

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  camera.setContext(ImGui::GetCurrentContext());
  ImGui_Vulkan_Init(vulkanInstance);

  // VkVP::ImGuiVulkanData ivData(vulkanInstance.vulkanDevice);
  VkVP::ImGuiUI ui(vulkanInstance.vulkanDevice, SHADER_DIR, fontPath);
  ui.initialize(vulkanInstance.queue,vulkanInstance.renderPass, width, height);

  /* Render-loop variables */
  uint32_t currentBufferIdx;
  float frameTimer;
  auto tStart = std::chrono::high_resolution_clock::now();
  auto &drawCmdBuffers = vulkanInstance.drawCmdBuffers;
  auto &renderPass = vulkanInstance.renderPass;
  auto& lightPos = vulkanInstance.projection.data.lightPos;
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
    ui.newFrame(frameTimer, camera);
    ui.updateBuffers();
    // lightPos = glm::vec4(camera.position, lightPos.w);
    updateProjectionBuffer(vulkanInstance.projection.buffer,
                           vulkanInstance.projection.data, camera, true);
    if (VkVP::getFrameBufferSizeChange(vulkanInstance.glfwWindow, width, height)) {
      VkVP::resizeImGuiWindow(vulkanInstance, width, height);
    }

    for (int i = 0; i < drawCmdBuffers.size(); i++) {
      initializers::beginCommandBuffer(drawCmdBuffers[i]);
      VkVP::beginRenderPass(renderPass, drawCmdBuffers[i], vulkanInstance.frameBuffers[i], width, height);
      std::for_each(instancePipelines.begin(), instancePipelines.end(),
                    [&](auto &pipeline) {
                      buildCommandBuffer(*pipeline, drawCmdBuffers[i]);
                    });
      ui.drawFrame(drawCmdBuffers[i]);
      vkCmdEndRenderPass(drawCmdBuffers[i]);
      VK_CHECK_RESULT(vkEndCommandBuffer(drawCmdBuffers[i]));
    }

    submitBuffers(vulkanInstance, currentBufferIdx);
  }
  VK_CHECK_RESULT(vkDeviceWaitIdle(vulkanDevice->logicalDevice));
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

                      
  vkDestroyDescriptorPool(vulkanDevice->logicalDevice,
                          vulkanInstance.descriptorPool, NULL);
  ImGui_ImplVulkanH_DestroyWindow(vulkanInstance.instance,
                                  vulkanDevice->logicalDevice,
                                  &vulkanInstance.ImGuiWindow, NULL);



#ifdef IMGUI_VULKAN_DEBUG_REPORT
  // Remove the debug report callback
  auto vkDestroyDebugReportCallbackEXT =
      (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(
          g_Instance, "vkDestroyDebugReportCallbackEXT");
  vkDestroyDebugReportCallbackEXT(g_Instance, g_DebugReport, g_Allocator);
#endif // IMGUI_VULKAN_DEBUG_REPORT

  glfwDestroyWindow(vulkanInstance.glfwWindow);
  glfwTerminate();
  // destroyVulkanInstance(vulkanInstance);
  return 0;
}