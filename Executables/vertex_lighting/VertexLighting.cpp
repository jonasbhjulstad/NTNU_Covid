#define KTX_OPENGL_ES3 1
#define ENABLE_VALIDATION true
#define GLFW_INCLUDE_VULKAN
// #include <VulkanViewport/VulkanglTFModel.h>
#include "menu.hpp"
#include <GLFW/glfw3.h>
#include <VulkanTools/Camera.hpp>
#include <VulkanTools/InstanceRendering/InstancePipeline.hpp>
#include <VulkanTools/ProjectionBuffer.hpp>
#include <VulkanTools/Renderpass.hpp>
#include <VulkanTools/Setup.hpp>
#include <VulkanTools/Submit.hpp>
#include <VulkanTools/Window.hpp>
#include <VulkanTools/gltf/glTFModel.hpp>
#include <VulkanViewport/Filepaths.hpp>
#include <VulkanViewport/Instance/Voxel.hpp>
#include <VulkanViewport/UI/ImGuiUI.hpp>
#include <chrono>
#include <memory>
#include <random>

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
VkDescriptorSetLayout setupDescriptorSetLayout(VkDevice device,
                                               bool with_texture) {

  std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {
      // Binding 0 : Vertex shader uniform buffer
      initializers::descriptorSetLayoutBinding(
          VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
      initializers::descriptorSetLayoutBinding(
          VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1)};
  if (with_texture)
    setLayoutBindings.push_back(initializers::descriptorSetLayoutBinding(
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT,
        2));

  VkDescriptorSetLayout descriptorSetLayout;
  VkDescriptorSetLayoutCreateInfo descriptorLayout =
      initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
  VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayout,
                                              nullptr, &descriptorSetLayout));

  return descriptorSetLayout;
}
VkDescriptorSet setupDescriptorSets(VkDevice device,
                                    VkDescriptorSetLayout descriptorSetLayout,
                                    VkDescriptorPool descriptorPool,
                                    VulkanBuffer *uniformProjectionBuffer,
                                    VulkanBuffer *lightingBuffer,
                                    Texture *texture) {
  VkDescriptorSet descriptorSet;
  VkDescriptorSetAllocateInfo allocInfo =
      initializers::descriptorSetAllocateInfo(descriptorPool,
                                              &descriptorSetLayout, 1);

  VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet));

  std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
      initializers::writeDescriptorSet(descriptorSet,
                                       VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0,
                                       &uniformProjectionBuffer->descriptor),
      initializers::writeDescriptorSet(descriptorSet,
                                       VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,
                                       &uniformProjectionBuffer->descriptor)};

  if (texture) {
    writeDescriptorSets.push_back(initializers::writeDescriptorSet(
        descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2,
        &texture->descriptor));
  }
  vkUpdateDescriptorSets(device,
                         static_cast<uint32_t>(writeDescriptorSets.size()),
                         writeDescriptorSets.data(), 0, nullptr);

  return descriptorSet;
}

std::unique_ptr<VkVP::InstancePipelineData>
prepareInstancePipeline(VulkanInstance &vulkanInstance,
                        VkVP::InstanceRenderingParams &p,
                        std::vector<VkVP::VoxelInstanceData> &instanceData,
                        VulkanBuffer *lightingBuffer) {
  using namespace VkVP;
  auto &device = p.vulkanDevice->logicalDevice;
  std::unique_ptr<InstancePipelineData> instancePipelineData =
      std::make_unique<InstancePipelineData>(device);

  instancePipelineData->model = loadModel(p.modelPath, p.vulkanDevice, p.queue);
  instancePipelineData->instanceBuffer = prepareInstanceBuffer(
      instanceData, p.vulkanDevice, p.queue, p.bufferUsageFlags);
  bool with_texture = !p.texturePath.empty();
  if (with_texture) {
    instancePipelineData->texture =
        loadTexture(p.texturePath, p.vulkanDevice, p.queue);
  }
  instancePipelineData->descriptorSetLayout =
      setupDescriptorSetLayout(device, with_texture);
  instancePipelineData->pipelineLayout =
      setupPipelineLayout(device, instancePipelineData->descriptorSetLayout);
  instancePipelineData->descriptorSet = setupDescriptorSets(
      device, instancePipelineData->descriptorSetLayout, p.descriptorPool,
      p.uniformProjectionBuffer, lightingBuffer, nullptr);
  instancePipelineData->pipeline = setupPipeline<VkVP::VoxelInstanceData>(
      p.vertexShaderPath, p.fragmentShaderPath,
      instancePipelineData->pipelineLayout, instancePipelineData->shaderStages,
      device, p.renderPass, p.pipelineCache);
  instancePipelineData->offset = p.offset;
  instancePipelineData->N_instances = instanceData.size();
  return instancePipelineData;
}
LightingData lightingData;
void prepareLightingBuffer(VulkanDevice *vulkanDevice, VulkanBuffer *buffer,
                           LightingData &lightingData) {
  VK_CHECK_RESULT(
      vulkanDevice->createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                 buffer, sizeof(LightingData)));
  // Map persistent
  VK_CHECK_RESULT(buffer->map());
  memcpy(buffer->mapped, &lightingData, sizeof(LightingData));
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
  vulkanInstance.glfwWindow =
      VkVP::createGLFWWindow(width, height, "VoxelGround");
  createVulkanInstance(
      ENABLE_VALIDATION, "VoxelGround", vulkanInstance.instance,
      vulkanInstance.supportedInstanceExtensions,
      vulkanInstance.enabledInstanceExtensions, VK_API_VERSION_1_0);
  setupVulkanPhysicalDevice(vulkanInstance, ENABLE_VALIDATION);
  VulkanDevice *vulkanDevice = vulkanInstance.vulkanDevice;
  VK_CHECK_RESULT(glfwCreateWindowSurface(vulkanInstance.instance,
                                          vulkanInstance.glfwWindow, nullptr,
                                          &vulkanInstance.surface));
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

  setupDescriptorPool(vulkanDevice->logicalDevice,
                      vulkanInstance.descriptorPool);
  VkDeviceSize offset[1] = {0};
  std::vector<VkVP::VoxelInstanceData> voxelInstanceData = {
      VkVP::VoxelInstanceData{{0, 0, 0}, {0, 0, 0}, glm::mat3(1.0f)}};
  VkVP::InstanceRenderingParams p;
  p.vertexShaderPath = SHADER_DIR + "cube.vert.spv";
  p.fragmentShaderPath = SHADER_DIR + "cube.frag.spv";
  p.modelPath = MODEL_DIR + "monkey.gltf";
  p.vulkanDevice = vulkanInstance.vulkanDevice;
  p.uniformProjectionBuffer = &vulkanInstance.projection.buffer;
  p.queue = vulkanInstance.queue;
  p.renderPass = vulkanInstance.renderPass;
  p.pipelineCache = vulkanInstance.pipelineCache;
  p.descriptorPool = vulkanInstance.descriptorPool;
  p.offset = offset;
  auto &device = p.vulkanDevice->logicalDevice;
  // prepare instance rendering
  VulkanBuffer *lightingBuffer;
  prepareLightingBuffer(vulkanDevice, lightingBuffer, lightingData);
  std::unique_ptr<InstancePipelineData> pipelineData = prepareInstancePipeline(
      vulkanInstance, p, voxelInstanceData, lightingBuffer);

  /* ImGUI App Initialization */

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  camera.setContext(ImGui::GetCurrentContext());
  ImGui_Vulkan_Init(vulkanInstance);

  // VkVP::ImGuiVulkanData ivData(vulkanInstance.vulkanDevice);
  VkVP::ImGuiUI ui(vulkanInstance.vulkanDevice, SHADER_DIR, fontPath);
  ui.initialize(vulkanInstance.queue, vulkanInstance.renderPass, width, height);

  /* Render-loop variables */
  uint32_t currentBufferIdx;
  float frameTimer;
  auto tStart = std::chrono::high_resolution_clock::now();
  auto &drawCmdBuffers = vulkanInstance.drawCmdBuffers;
  auto &renderPass = vulkanInstance.renderPass;
  auto &lightPos = vulkanInstance.projection.data.lightPos;
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
    showLightSettings(lightingData);
    memcpy(lightingBuffer->mapped, &lightingData, sizeof(LightingData));

    if (VkVP::getFrameBufferSizeChange(vulkanInstance.glfwWindow, width,
                                       height)) {
      VkVP::resizeImGuiWindow(vulkanInstance, width, height);
    }

    for (int i = 0; i < drawCmdBuffers.size(); i++) {
      initializers::beginCommandBuffer(drawCmdBuffers[i]);
      VkVP::beginRenderPass(renderPass, drawCmdBuffers[i],
                            vulkanInstance.frameBuffers[i], width, height);
      buildCommandBuffer(*pipelineData, drawCmdBuffers[i]);
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