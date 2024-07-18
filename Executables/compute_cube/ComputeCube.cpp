#define KTX_OPENGL_ES3 1
#define ENABLE_VALIDATION true

// #include <VulkanViewport/VulkanglTFModel.h>
#include "compute.hpp"
#include "graphics.hpp"
#include "particle.hpp"
#include <GLFW/glfw3.h>
#include <VulkanTools/Camera.hpp>
#include <VulkanTools/ProjectionBuffer.hpp>
#include <VulkanTools/Setup.hpp>
#include <VulkanTools/Window.hpp>
#include <VulkanTools/gltf/Texture.hpp>
#include <VulkanTools/gltf/glTFModel.hpp>
#include <VulkanTools/Instance.hpp>
#include <VulkanViewport/Filepaths.hpp>
#include <VulkanViewport/Instance/Voxel.hpp>
#include <VulkanViewport/InstanceRendering/SetupRoutines.hpp>
#include <VulkanViewport/UI/ImGuiUI.hpp>
#include <VulkanViewport/UI/UISettings.hpp>
#include <chrono>

// Particle Definition
VkDescriptorPool descriptorPool;
uint32_t numParticles{0};
Compute compute;
// We use a shader storage buffer object to store the particlces
// This is updated by the compute pipeline and displayed as a vertex buffer by
// the graphics pipeline
VulkanBuffer storageBuffer;

// Resources for the graphics part of the example

// Resources for the compute part of the example

void loadAssets(VulkanInstance &vulkanInstance) {
  textures.particle.loadFromFile(
      VkVP::TEXTURE_DIR + "/particle01_rgba.ktx", VK_FORMAT_R8G8B8A8_UNORM,
      vulkanInstance.vulkanDevice, vulkanInstance.queue);
  textures.gradient.loadFromFile(
      VkVP::TEXTURE_DIR + "/particle_gradient_rgba.ktx",
      VK_FORMAT_R8G8B8A8_UNORM, vulkanInstance.vulkanDevice,
      vulkanInstance.queue);
}

void prepare(VulkanInstance &vulkanInstance, VkDescriptorPool &descriptorPool,
             std::vector<Particle> &particleBuffer, uint32_t width,
             uint32_t height) {
  uint32_t graphics_QFI =
      vulkanInstance.vulkanDevice->queueFamilyIndices.graphics;
  compute.queueFamilyIndex =
      vulkanInstance.vulkanDevice->queueFamilyIndices.compute;
  compute.queue = vulkanInstance.queue;
  loadAssets(vulkanInstance);
  prepareStorageBuffers(vulkanInstance, compute, particleBuffer, storageBuffer, graphics.queueFamilyIndex);
  // prepareGraphics(vulkanInstance, , descriptorPool);
  prepareCompute(vulkanInstance, compute, storageBuffer, descriptorPool);
}

void submitFrame(VulkanInstance &vulkanInstance, uint32_t currentBuffer) {
  VkResult result = vulkanInstance.swapChain.queuePresent(
      vulkanInstance.queue, currentBuffer,
      vulkanInstance.semaphores.renderComplete);
  // Recreate the swapchain if it's no longer compatible with the surface
  // (OUT_OF_DATE) or no longer optimal for presentation (SUBOPTIMAL)
  //   if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result ==
  //   VK_SUBOPTIMAL_KHR)) {
  //     windowResize();
  //     if (result == VK_ERROR_OUT_OF_DATE_KHR) {
  //       return;
  //     }
  //   } else {
  VK_CHECK_RESULT(result);
  VK_CHECK_RESULT(vkQueueWaitIdle(vulkanInstance.queue));
}
void particles_draw(VulkanInstance &vulkanInstance, uint32_t &currentBuffer) {

  VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

  // Submit compute commands
  VkSubmitInfo computeSubmitInfo = initializers::submitInfo();
  computeSubmitInfo.commandBufferCount = 1;
  computeSubmitInfo.pCommandBuffers = &compute.commandBuffer;
  computeSubmitInfo.waitSemaphoreCount = 1;
  computeSubmitInfo.pWaitSemaphores = &graphics.semaphore;
  computeSubmitInfo.pWaitDstStageMask = &waitStageMask;
  computeSubmitInfo.signalSemaphoreCount = 1;
  computeSubmitInfo.pSignalSemaphores = &compute.semaphore;
  VK_CHECK_RESULT(
      vkQueueSubmit(compute.queue, 1, &computeSubmitInfo, VK_NULL_HANDLE));

  VkResult result = vulkanInstance.swapChain.acquireNextImage(
      vulkanInstance.semaphores.presentComplete, &currentBuffer);

  VkPipelineStageFlags graphicsWaitStageMasks[] = {
      VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  VkSemaphore graphicsWaitSemaphores[] = {
      compute.semaphore, vulkanInstance.semaphores.presentComplete};
  VkSemaphore graphicsSignalSemaphores[] = {
      graphics.semaphore, vulkanInstance.semaphores.renderComplete};
  auto submitInfo = initializers::submitInfo();
  // Submit graphics commands
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &vulkanInstance.drawCmdBuffers[currentBuffer];
  submitInfo.waitSemaphoreCount = 2;
  submitInfo.pWaitSemaphores = graphicsWaitSemaphores;
  submitInfo.pWaitDstStageMask = graphicsWaitStageMasks;
  submitInfo.signalSemaphoreCount = 2;
  submitInfo.pSignalSemaphores = graphicsSignalSemaphores;
  VK_CHECK_RESULT(
      vkQueueSubmit(vulkanInstance.queue, 1, &submitInfo, VK_NULL_HANDLE));
  submitFrame(vulkanInstance, currentBuffer);
}
void recreateCommandBuffers(VulkanInstance& vulkanInstance, uint32_t width, uint32_t height)
{
    VkDevice logicalDevice = vulkanInstance.vulkanDevice->logicalDevice;
    // Ensure all operations on the device have been finished before destroying resources
    vkDeviceWaitIdle(logicalDevice);
    vulkanInstance.swapChain.create(&vulkanInstance.ImGuiWindow, (uint32_t *)&width, (uint32_t *)&height, false);
    vulkanInstance.ImGuiWindow.Swapchain = vulkanInstance.swapChain.swapChain;
    // Recreate the frame buffersqueuePresent
    vkDestroyImageView(logicalDevice, vulkanInstance.depthStencil.view, nullptr);
    vkDestroyImage(logicalDevice, vulkanInstance.depthStencil.image, nullptr);
    vkFreeMemory(logicalDevice, vulkanInstance.depthStencil.mem, nullptr);
    initializers::setupDepthStencil(vulkanInstance, width, height);
    for (uint32_t i = 0; i < vulkanInstance.frameBuffers.size(); i++)
    {
        vkDestroyFramebuffer(logicalDevice, vulkanInstance.frameBuffers[i], nullptr);
    }
    initializers::setupFrameBuffer(logicalDevice,
                                   vulkanInstance.renderPass,
                                   width,
                                   height,
                                   vulkanInstance.depthStencil.view,
                                   vulkanInstance.swapChain,
                                   vulkanInstance.frameBuffers);

    // Command buffers need to be recreated as they may store
    // references to the recreated frame buffer
    initializers::destroyCommandBuffers(logicalDevice, vulkanInstance.vulkanDevice->commandPool, vulkanInstance.drawCmdBuffers);
    initializers::createCommandBuffers(logicalDevice, vulkanInstance.drawCmdBuffers, vulkanInstance.swapChain, vulkanInstance.vulkanDevice->commandPool);
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
      glfwCreateWindow(width, height, "VoxelGround", nullptr, nullptr);

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

  createVulkanInstance(
      ENABLE_VALIDATION, "VoxelGround", vulkanInstance.instance,
      vulkanInstance.supportedInstanceExtensions,
      vulkanInstance.enabledInstanceExtensions, VK_API_VERSION_1_0);
  setupVulkanPhysicalDevice(vulkanInstance, ENABLE_VALIDATION);

  VK_CHECK_RESULT(glfwCreateWindowSurface(vulkanInstance.instance,
                                          vulkanInstance.glfwWindow, nullptr,
                                          &vulkanInstance.surface));

  VulkanDevice *vulkanDevice = vulkanInstance.vulkanDevice;
  setupGLFWVulkanWindow(vulkanInstance, width, height, 2);
  prepareVulkan(vulkanInstance, width, height);

  /* User Interface Settings */

  UISettings uiSettings;
  uiSettings.fontPath = VkVP::ASSET_DIR + "fonts/DroidSansMono.ttf";
  Camera camera;
  camera.type = camera.firstperson;
  camera.position = glm::vec3(0.0f, 0.0f, -10.0f);
  camera.rotation = glm::vec3(-45.0f, 0.0f, 0.0f);
  camera.setPerspective(60.0f, (float)width / (float)height, 0.1f, 1000.0f);

  using namespace VkVP;

  prepareProjectionBuffer(vulkanDevice, vulkanInstance.projection.buffer,
                          vulkanInstance.projection.data, camera);

  VkDeviceSize offset[1] = {0};
  uint32_t particles_per_attractor = 128;
  std::vector<Particle> particleBuffer =
      create_particles(particles_per_attractor);
  VkVP::ImGuiVulkanData ivData(vulkanInstance.vulkanDevice);



  /* ImGUI App Initialization */

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiContext *g = ImGui::GetCurrentContext();
  camera.setContext(g);
  ImGuiIO &io = ImGui::GetIO();
  camera.mousePos_old = {io.MousePos.x, io.MousePos.y};
  ImGui_Vulkan_Init(vulkanInstance);

  // camera.setWindowID(ImGui::GetCurrentWindow());


  VkVP::setupImGuiVisuals(width, height, uiSettings);

  VkVP::initializeImGuiVulkanResources(ivData, vulkanInstance.renderPass,
                                       vulkanInstance.queue, VkVP::SHADER_DIR);

  /* Render-loop variables */
  bool rebuildSwapChain = false;
  uint32_t currentBufferIdx;
  float frameTimer;
  auto tStart = std::chrono::high_resolution_clock::now();
  uint32_t currentBuffer = 0;

  
  //instance data preparation
  auto voxelInstanceData = generate_ground(1, 1, 11);
  // voxelInstanceData[0].scale.x = 10.0f;
  VkVP::InstanceRenderingParams voxelParams;
  voxelParams.vertexShaderPath = VkVP::SHADER_DIR + "cube.vert.spv";
  voxelParams.fragmentShaderPath = VkVP::SHADER_DIR + "cube.frag.spv";
  voxelParams.modelPath = VkVP::MODEL_DIR + "cube.gltf";
  voxelParams.vulkanDevice = vulkanInstance.vulkanDevice;
  voxelParams.uniformProjectionBuffer = &vulkanInstance.projection.buffer;
  voxelParams.queue = vulkanInstance.queue;
  voxelParams.renderPass = vulkanInstance.renderPass;
  voxelParams.pipelineCache = vulkanInstance.pipelineCache;
  voxelParams.descriptorPool = descriptorPool;
  voxelParams.offset = offset;

  std::vector<std::unique_ptr<InstancePipelineData>> instancePipelines;
  instancePipelines.push_back(prepareInstanceRendering<VoxelInstanceData>(voxelParams, voxelInstanceData));



  prepare(vulkanInstance, descriptorPool, particleBuffer, width, height);
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
    VkVP::newFrame(uiSettings, frameTimer, camera);
    VkVP::updateBuffers(vulkanInstance.vulkanDevice, ivData.vertexBuffer, ivData.indexBuffer, ivData.indexCount, ivData.vertexCount);
    updateProjectionBuffer(vulkanInstance.projection.buffer, vulkanInstance.projection.data, camera, true);

    updateGraphicsUniformBuffers(graphics, camera, width, height);
    updateComputeUniformBuffers(compute, frameTimer);

    updateWindowSize(vulkanInstance, ivData, camera, instancePipelines,  width, height);

    // recreateCommandBuffers(vulkanInstance, width, height);
    // buildCommandBuffers(vulkanInstance, graphics, vulkanInstance.drawCmdBuffers,
    //                     storageBuffer, ivData, particleBuffer.size(), width, height, compute.queueFamilyIndex);
    buildComputeCommandBuffer(compute, storageBuffer, particleBuffer.size(), graphics.queueFamilyIndex);
    particles_draw(vulkanInstance, currentBuffer);
  }

  ImGui_ImplVulkanH_DestroyWindow(vulkanInstance.instance,
                                  vulkanDevice->logicalDevice,
                                  &vulkanInstance.ImGuiWindow, nullptr);
  vkDestroyDescriptorPool(vulkanDevice->logicalDevice,
                          vulkanInstance.descriptorPool, nullptr);

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