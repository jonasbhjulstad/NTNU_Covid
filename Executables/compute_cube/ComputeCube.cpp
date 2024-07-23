#define KTX_OPENGL_ES3 1
#define ENABLE_VALIDATION true
#define GLFW_INCLUDE_VULKAN
// #include <VulkanViewport/VulkanglTFModel.h>
#include <GLFW/glfw3.h>
#include <VulkanTools/Camera.hpp>
#include <VulkanTools/InstanceRendering/InstancePipeline.hpp>
#include <VulkanTools/ProjectionBuffer.hpp>
#include <VulkanTools/Renderpass.hpp>
#include <VulkanTools/Setup.hpp>
#include <VulkanTools/Submit.hpp>
#include <VulkanTools/Window.hpp>
#include <VulkanTools/gltf/glTFModel.hpp>
#include <VulkanViewport/Compute/Common/synchronization.hpp>
#include <VulkanViewport/Compute/Cube/compute.hpp>
#include <VulkanViewport/Compute/Cube/graphics.hpp>
#include <VulkanViewport/Compute/Cube/particle.hpp>
#include <VulkanViewport/Filepaths.hpp>
#include <VulkanViewport/UI/ImGuiUI.hpp>
#include <chrono>
#include <memory>
constexpr bool ENABLE_COMPUTE_QUEUE = true;
constexpr uint32_t PARTICLE_RENDER_IMAGE_COUNT = 3;
uint32_t numParticles{0};
VkVP::Cube::Compute compute;

void setupDescriptorPool(VkDevice logicalDevice,
                         VkDescriptorPool &descriptorPool) {
  // Example uses one ubo
  std::vector<VkDescriptorPoolSize> poolSizes = {
      initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2),
      initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1),
      initializers::descriptorPoolSize(
          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 3),
  };

  VkDescriptorPoolCreateInfo descriptorPoolInfo =
      initializers::descriptorPoolCreateInfo(poolSizes.size(), poolSizes.data(),
                                             3);
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
  vulkanInstance.glfwWindow =
      VkVP::createGLFWWindow(width, height, "ComputeCube");
  // vulkanInstance.enabledInstanceExtensions.push_back(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME);
  createVulkanInstance(
      ENABLE_VALIDATION, "VoxelGround", vulkanInstance.instance,
      vulkanInstance.supportedInstanceExtensions,
      vulkanInstance.enabledInstanceExtensions, VK_API_VERSION_1_2);
  vulkanInstance.enabledInstanceExtensions.push_back(
      VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME);
  //   std::for_each(vulkanInstance.supportedInstanceExtensions.begin(),
  //                 vulkanInstance.supportedInstanceExtensions.end(),
  //                 [](auto &ext) { std::cout << ext << std::endl; });

  setupVulkanPhysicalDevice(vulkanInstance, ENABLE_VALIDATION,
                            ENABLE_COMPUTE_QUEUE);

  VK_CHECK_RESULT(glfwCreateWindowSurface(vulkanInstance.instance,
                                          vulkanInstance.glfwWindow, nullptr,
                                          &vulkanInstance.surface));

  VulkanDevice *vulkanDevice = vulkanInstance.vulkanDevice;
  prepareVulkan(vulkanInstance, width, height);
  VkVP::setupGLFWVulkanWindow(vulkanInstance, width, height,
                              PARTICLE_RENDER_IMAGE_COUNT);

  /* User Interface Settings */

  std::string fontPath = VkVP::ASSET_DIR + "fonts/DroidSansMono.ttf";
  Camera camera;
  camera.type = camera.firstperson;
  camera.position = glm::vec3(0.0f, 0.0f, -10.0f);
  camera.rotation = glm::vec3(-45.0f, 0.0f, 0.0f);
  camera.setPerspective(60.0f, (float)width / (float)height, 0.1f, 1000.0f);

  using namespace VkVP;

  prepareProjectionBuffer(vulkanDevice, vulkanInstance.projection.buffer,
                          vulkanInstance.projection.data, camera);

  VkDeviceSize offset[1] = {0};
  uint32_t particles_per_attractor = 8;
  std::vector<VkVP::Cube::Particle> particles =
      VkVP::Cube::createParticles(particles_per_attractor);
  uint32_t N_particles = particles.size();

  /* ImGUI App Initialization */

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiContext *g = ImGui::GetCurrentContext();
  camera.setContext(g);
  ImGuiIO &io = ImGui::GetIO();
  camera.mousePos_old = {io.MousePos.x, io.MousePos.y};
  ImGui_Vulkan_Init(vulkanInstance);

  // camera.setWindowID(ImGui::GetCurrentWindow());

  float frameTimer = 0.0f;
  VkVP::ImGuiUI ui(vulkanInstance.vulkanDevice, SHADER_DIR, fontPath);
  ui.initialize(vulkanInstance.queue, vulkanInstance.renderPass, width, height);
  auto tStart = std::chrono::high_resolution_clock::now();
  auto &drawCmdBuffers = vulkanInstance.drawCmdBuffers;
  auto &renderPass = vulkanInstance.renderPass;
  //   prepare(vulkanInstance, particles, width, height);
  uint32_t graphics_QFI =
      vulkanInstance.vulkanDevice->queueFamilyIndices.graphics;
  uint32_t compute_QFI =
      vulkanInstance.vulkanDevice->queueFamilyIndices.compute;

  uint32_t currentBuffer = 0;
  uint32_t cubes_per_attractor = 128;
  // instance data preparation
  auto particleInstanceData = VkVP::Cube::createParticles(cubes_per_attractor);
  VkVP::InstanceRenderingParams voxelParams;
  voxelParams.vertexShaderPath =
      VkVP::SHADER_DIR + "/ComputeCube/compute_cube.vert.spv";
  voxelParams.fragmentShaderPath =
      VkVP::SHADER_DIR + "/ComputeCube/compute_cube.frag.spv";
  voxelParams.modelPath = VkVP::MODEL_DIR + "cube.gltf";
  voxelParams.vulkanDevice = vulkanInstance.vulkanDevice;
  voxelParams.uniformProjectionBuffer = &vulkanInstance.projection.buffer;
  voxelParams.queue = vulkanInstance.queue;
  voxelParams.renderPass = vulkanInstance.renderPass;
  voxelParams.pipelineCache = vulkanInstance.pipelineCache;
  voxelParams.descriptorPool = vulkanInstance.descriptorPool;
  voxelParams.offset = offset;
  // Configure storage for compute / graphics interop
  voxelParams.bufferUsageFlags =
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
  compute.queueFamilyIndex =
      vulkanInstance.vulkanDevice->queueFamilyIndices.compute;
  compute.queue = vulkanInstance.queue;
  setupDescriptorPool(vulkanDevice->logicalDevice,
                      vulkanInstance.descriptorPool);
    std::unique_ptr<VkVP::InstancePipelineData> cubePipeline = prepareSynchronizedInstancePipeline(vulkanInstance, voxelParams, particles, graphics_QFI, compute_QFI);
  VulkanBuffer &instanceBuffer = cubePipeline->instanceBuffer;
  compute.commandPool = vulkanInstance.vulkanDevice->commandPool;
  VkVP::Cube::prepareCompute(vulkanInstance, compute, instanceBuffer,
                             vulkanInstance.descriptorPool);
  auto device = vulkanInstance.vulkanDevice->logicalDevice;
//   {
//     auto semaCI = initializers::semaphoreCreateInfo();
//     VK_CHECK_RESULT(vkCreateSemaphore(device, &semaCI, nullptr,
//                                       &compute.semaphores.compute));
//     VK_CHECK_RESULT(vkCreateSemaphore(device, &semaCI, nullptr,
//                                       &compute.semaphores.graphics));
    

//     if (graphics_QFI != compute_QFI) {
//       initializers::beginCommandBuffer(compute.commandBuffer);
//       VkBufferMemoryBarrier buffer_barrier = {
//           VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
//           nullptr,
//           VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
//           0,
//           graphics_QFI,
//           compute.queueFamilyIndex,
//           instanceBuffer.buffer,
//           0,
//           instanceBuffer.size};

//       vkCmdPipelineBarrier(compute.commandBuffer, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
//                            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr,
//                            1, &buffer_barrier, 0, nullptr);
//         vulkanDevice->flushCommandBuffer(compute.commandBuffer, compute.queue, true);
//     }
//   }
  VkSubmitInfo submitInfo = initializers::submitInfo();
  submitInfo.pSignalSemaphores = &compute.semaphores.graphics;
  submitInfo.signalSemaphoreCount = 1;
  VK_CHECK_RESULT(
      vkQueueSubmit(vulkanInstance.queue, 1, &submitInfo, VK_NULL_HANDLE));


  while (!glfwWindowShouldClose(vulkanInstance.glfwWindow)) {
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    // Start the Dear ImGui frame
    camera.update(frameTimer);
    auto tEnd = std::chrono::high_resolution_clock::now();
    auto tDiff =
        std::chrono::duration<double, std::milli>(tEnd - tStart).count();
    frameTimer = (float)tDiff / 1000.0f;
    tStart = tEnd;
    ui.newFrame(frameTimer, camera);
    ui.updateBuffers();
    updateProjectionBuffer(vulkanInstance.projection.buffer,
                           vulkanInstance.projection.data, camera, true);
    VkVP::Cube::updateComputeUniformBuffers(compute, frameTimer);
    if (VkVP::getFrameBufferSizeChange(vulkanInstance.glfwWindow, width,
                                       height)) {
      VkVP::resizeImGuiWindow(vulkanInstance, width, height);
    }
    for (int i = 0; i < drawCmdBuffers.size(); i++) {
      initializers::beginCommandBuffer(drawCmdBuffers[i]);
      VkVP::Compute::graphicsMemoryBarrierAquire(
          instanceBuffer, drawCmdBuffers[i], graphics_QFI, compute_QFI);
      VkVP::beginRenderPass(renderPass, drawCmdBuffers[i],
                            vulkanInstance.frameBuffers[i], width, height);
      VkVP::buildCommandBuffer(*cubePipeline, drawCmdBuffers[i]);
      ui.drawFrame(drawCmdBuffers[i]);
      vkCmdEndRenderPass(drawCmdBuffers[i]);
      VkVP::Compute::graphicsMemoryBarrierRelease(
          instanceBuffer, drawCmdBuffers[i], graphics_QFI, compute_QFI);
      VK_CHECK_RESULT(vkEndCommandBuffer(drawCmdBuffers[i]));
    }

    // Build Compute
    initializers::beginCommandBuffer(compute.commandBuffer);
    VkVP::Compute::computeMemoryBarrierAquire(
        instanceBuffer, compute.commandBuffer, graphics_QFI, compute_QFI);
    buildComputeCommandBuffer(compute, instanceBuffer, N_particles,
                              graphics_QFI);
    VkVP::Compute::computeMemoryBarrierRelease(
        instanceBuffer, compute.commandBuffer, graphics_QFI, compute_QFI);
    vkEndCommandBuffer(compute.commandBuffer);

    // Submit all buffers
    VkVP::submitBuffers(vulkanInstance, compute.commandBuffer,
                        compute.semaphores.graphics,
                        compute.semaphores.compute);
  }

  ImGui_ImplVulkanH_DestroyWindow(vulkanInstance.instance,
                                  vulkanDevice->logicalDevice,
                                  &vulkanInstance.ImGuiWindow, nullptr);
  vkDestroyDescriptorPool(vulkanDevice->logicalDevice,
                          vulkanInstance.descriptorPool, nullptr);

  glfwDestroyWindow(vulkanInstance.glfwWindow);
  glfwTerminate();

  return 0;
}