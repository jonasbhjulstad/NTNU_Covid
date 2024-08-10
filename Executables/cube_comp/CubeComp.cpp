#define KTX_OPENGL_ES3 1
#define ENABLE_VALIDATION true
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <VulkanTools/Camera.hpp>
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
#include <tuple>

constexpr bool ENABLE_COMPUTE_QUEUE = true;
constexpr uint32_t PARTICLE_RENDER_IMAGE_COUNT = 3;
uint32_t numParticles{0};
// VkVP::Cube::Graphics graphics;
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

void prepare(VulkanInstance &vulkanInstance, VulkanBuffer &instanceBuffer,
             std::vector<VkVP::Cube::Particle> &particles, uint32_t width,
             uint32_t height) {
  using namespace VkVP::Cube;
  uint32_t graphics_QFI =
      vulkanInstance.vulkanDevice->queueFamilyIndices.graphics;
  uint32_t compute_QFI =
      vulkanInstance.vulkanDevice->queueFamilyIndices.compute;
  compute.queueFamilyIndex = compute_QFI;
  //   prepareinstanceBuffers(vulkanInstance, compute, particles,
  //   instanceBuffer,
  // graphics_QFI);
  VkVP::Cube::prepareCompute(vulkanInstance, compute, instanceBuffer,
                             vulkanInstance.descriptorPool);
}

auto imgui_setup(VulkanInstance &vulkanInstance, uint32_t width,
                 uint32_t height, Camera &camera) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiContext *g = ImGui::GetCurrentContext();
  camera.setContext(g);
  ImGuiIO &io = ImGui::GetIO();
  camera.mousePos_old = {io.MousePos.x, io.MousePos.y};
  ImGui_Vulkan_Init(vulkanInstance);
  std::string fontPath = VkVP::ASSET_DIR + "fonts/DroidSansMono.ttf";

  VkVP::ImGuiUI ui(vulkanInstance.vulkanDevice, VkVP::SHADER_DIR, fontPath);
  ui.initialize(vulkanInstance.queue, vulkanInstance.renderPass, width, height);
  return ui;
}
auto create_particles() {
  uint32_t particles_per_attractor = 128;
  std::vector<VkVP::Cube::Particle> particles =
      VkVP::Cube::createParticles(particles_per_attractor);
  return particles;
}

auto instance_setup(VulkanInstance &vulkanInstance,
                    std::vector<VkVP::Cube::Particle> &particles,
                    uint32_t width, uint32_t height) {
  VkDeviceSize offset[1] = {0};

  uint32_t N_particles = particles.size();

  uint32_t graphics_QFI =
      vulkanInstance.vulkanDevice->queueFamilyIndices.graphics;
  uint32_t compute_QFI =
      vulkanInstance.vulkanDevice->queueFamilyIndices.compute;

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

  std::unique_ptr<VkVP::InstancePipelineData> cubePipeline =
      prepareSynchronizedInstancePipeline(vulkanInstance, voxelParams,
                                          particles, graphics_QFI, compute_QFI);
  return cubePipeline;
}

int main() {
  VulkanInstance vulkanInstance;
  /* Setup, Integration and Initialization of Vulkan/GLFW */

  if (!glfwInit())
    return 1;

  uint32_t width = 1200;
  uint32_t height = 1200;
  VkVP::assignGLFWRequiredInstanceExtensions(vulkanInstance);
  vulkanInstance.glfwWindow =
      VkVP::createGLFWWindow(width, height, "ParticleCompute");
  createVulkanInstance(
      ENABLE_VALIDATION, "ParticleCompute", vulkanInstance.instance,
      vulkanInstance.supportedInstanceExtensions,
      vulkanInstance.enabledInstanceExtensions, VK_API_VERSION_1_0);
  setupVulkanPhysicalDevice(vulkanInstance, ENABLE_VALIDATION,
                            ENABLE_COMPUTE_QUEUE);

  VulkanDevice *vulkanDevice = vulkanInstance.vulkanDevice;
  VK_CHECK_RESULT(glfwCreateWindowSurface(vulkanInstance.instance,
                                          vulkanInstance.glfwWindow, nullptr,
                                          &vulkanInstance.surface));
  prepareVulkan(vulkanInstance, width, height);
  VkVP::setupGLFWVulkanWindow(vulkanInstance, width, height,
                              PARTICLE_RENDER_IMAGE_COUNT);
  /* User Interface Settings */

  setupDescriptorPool(vulkanDevice->logicalDevice,
                      vulkanInstance.descriptorPool);
  Camera camera;
  camera.type = camera.firstperson;
  camera.position = glm::vec3(0.0f, 0.0f, -10.0f);
  camera.rotation = glm::vec3(-45.0f, 0.0f, 0.0f);
  camera.setPerspective(60.0f, (float)width / (float)height, 0.1f, 1000.0f);

  prepareProjectionBuffer(vulkanDevice, vulkanInstance.projection.buffer,
                          vulkanInstance.projection.data, camera);

  auto ui = imgui_setup(vulkanInstance, width, height, camera);
  auto particles = create_particles();
  uint32_t N_particles = particles.size();
  auto cubePipeline = instance_setup(vulkanInstance, particles, width, height);
  compute.uniformData.particleCount = N_particles;
  VkDeviceSize offsets[1] = {0};
  cubePipeline->offset = offsets;
  VulkanBuffer &instanceBuffer = cubePipeline->instanceBuffer;

  uint32_t graphics_QFI =
      vulkanInstance.vulkanDevice->queueFamilyIndices.graphics;
  uint32_t compute_QFI =
      vulkanInstance.vulkanDevice->queueFamilyIndices.compute;

  float frameTimer;
  auto tStart = std::chrono::high_resolution_clock::now();
  auto &drawCmdBuffers = vulkanInstance.drawCmdBuffers;
  auto &renderPass = vulkanInstance.renderPass;
  prepare(vulkanInstance, instanceBuffer, particles, width, height);

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
    initializers::beginCommandBuffer(compute.commandBuffer);
    VkVP::Compute::computeMemoryBarrierAquire(
        instanceBuffer, compute.commandBuffer, graphics_QFI, compute_QFI);
    buildComputeCommandBuffer(compute, instanceBuffer, N_particles,
                              graphics_QFI);
    VkVP::Compute::computeMemoryBarrierRelease(
        instanceBuffer, compute.commandBuffer, graphics_QFI, compute_QFI);
    vkEndCommandBuffer(compute.commandBuffer);
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