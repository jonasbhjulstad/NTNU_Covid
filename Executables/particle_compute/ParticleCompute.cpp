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
#include <VulkanViewport/Compute/Particle/compute.hpp>
#include <VulkanViewport/Compute/Particle/graphics.hpp>
#include <VulkanViewport/Compute/Particle/particle.hpp>
#include <VulkanViewport/Compute/Common/synchronization.hpp>
#include <VulkanViewport/Filepaths.hpp>
#include <VulkanViewport/UI/ImGuiUI.hpp>
#include <chrono>

VkVP::Particle::ParticleTextures textures{};
constexpr bool ENABLE_COMPUTE_QUEUE = true;
constexpr uint32_t PARTICLE_RENDER_IMAGE_COUNT = 3;
uint32_t numParticles{0};
VkVP::Particle::Graphics graphics;
VkVP::Particle::Compute compute;
VulkanBuffer storageBuffer;

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

void loadAssets(VulkanInstance &vulkanInstance) {
  textures.particle.loadFromFile(
      VkVP::TEXTURE_DIR + "/particle01_rgba.ktx", VK_FORMAT_R8G8B8A8_UNORM,
      vulkanInstance.vulkanDevice, vulkanInstance.queue);
  textures.gradient.loadFromFile(
      VkVP::TEXTURE_DIR + "/particle_gradient_rgba.ktx",
      VK_FORMAT_R8G8B8A8_UNORM, vulkanInstance.vulkanDevice,
      vulkanInstance.queue);
}

void prepare(VulkanInstance &vulkanInstance,
             std::vector<VkVP::Particle::Particle> &particleBuffer,
             uint32_t width, uint32_t height) {
  using namespace VkVP::Particle;
  uint32_t graphics_QFI =
      vulkanInstance.vulkanDevice->queueFamilyIndices.graphics;
  compute.queueFamilyIndex =
      vulkanInstance.vulkanDevice->queueFamilyIndices.compute;
  compute.queue = vulkanInstance.queue;
  loadAssets(vulkanInstance);
  prepareStorageBuffers(vulkanInstance, compute, particleBuffer, storageBuffer,
                        graphics_QFI);
  VkVP::Particle::prepareCompute(vulkanInstance, compute, storageBuffer,
                                 vulkanInstance.descriptorPool);
  VkVP::Particle::prepareGraphics(vulkanInstance, graphics,
                                  vulkanInstance.descriptorPool, textures);
}

int main() {
  VulkanInstance vulkanInstance;

  /* Setup, Integration and Initialization of Vulkan/GLFW */

  if (!glfwInit())
    return 1;

  uint32_t width = 1280;
  uint32_t height = 720;
  VkVP::assignGLFWRequiredInstanceExtensions(vulkanInstance);
  vulkanInstance.glfwWindow =
      VkVP::createGLFWWindow(width, height, "ParticleCompute");
  createVulkanInstance(
      ENABLE_VALIDATION, "ParticleCompute", vulkanInstance.instance,
      vulkanInstance.supportedInstanceExtensions,
      vulkanInstance.enabledInstanceExtensions, VK_API_VERSION_1_0);
  setupVulkanPhysicalDevice(vulkanInstance, ENABLE_VALIDATION, ENABLE_COMPUTE_QUEUE);

  VulkanDevice *vulkanDevice = vulkanInstance.vulkanDevice;
  VK_CHECK_RESULT(glfwCreateWindowSurface(vulkanInstance.instance,
                                          vulkanInstance.glfwWindow, nullptr,
                                          &vulkanInstance.surface));
  prepareVulkan(vulkanInstance, width, height);
  VkVP::setupGLFWVulkanWindow(vulkanInstance, width, height, PARTICLE_RENDER_IMAGE_COUNT);

  /* User Interface Settings */
  std::string fontPath = VkVP::ASSET_DIR + "fonts/DroidSansMono.ttf";

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
  uint32_t particles_per_attractor = 128;
  std::vector<VkVP::Particle::Particle> particleBuffer =
      VkVP::Particle::create_particles(particles_per_attractor);
  uint32_t N_particles = particleBuffer.size();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiContext *g = ImGui::GetCurrentContext();
  camera.setContext(g);
  ImGuiIO &io = ImGui::GetIO();
  camera.mousePos_old = {io.MousePos.x, io.MousePos.y};
  ImGui_Vulkan_Init(vulkanInstance);

  VkVP::ImGuiUI ui(vulkanInstance.vulkanDevice, VkVP::SHADER_DIR, fontPath);
  ui.initialize(vulkanInstance.queue, vulkanInstance.renderPass, width, height);
  float frameTimer;
  auto tStart = std::chrono::high_resolution_clock::now();
  auto &drawCmdBuffers = vulkanInstance.drawCmdBuffers;
  auto &renderPass = vulkanInstance.renderPass;
  prepare(vulkanInstance, particleBuffer, width, height);
  uint32_t graphics_QFI =
      vulkanInstance.vulkanDevice->queueFamilyIndices.graphics;
  uint32_t compute_QFI =
      vulkanInstance.vulkanDevice->queueFamilyIndices.compute;

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
    VkVP::Particle::updateGraphicsUniformBuffers(graphics, camera, width,
                                                 height);
    VkVP::Particle::updateComputeUniformBuffers(compute, frameTimer);
    if (VkVP::getFrameBufferSizeChange(vulkanInstance.glfwWindow, width,
                                       height)) {
      VkVP::resizeImGuiWindow(vulkanInstance, width, height);
    }
    for (int i = 0; i < drawCmdBuffers.size(); i++) {
      initializers::beginCommandBuffer(drawCmdBuffers[i]);

      VkVP::Compute::graphicsMemoryBarrierAquire(
          storageBuffer, drawCmdBuffers[i], graphics_QFI, compute_QFI);
      VkVP::beginRenderPass(renderPass, drawCmdBuffers[i],
                            vulkanInstance.frameBuffers[i], width, height);
      VkVP::Particle::buildGraphicsCommandBuffer(
          vulkanInstance, graphics, drawCmdBuffers[i], storageBuffer,
          N_particles, width, height, compute_QFI);
      ui.drawFrame(drawCmdBuffers[i]);
      vkCmdEndRenderPass(drawCmdBuffers[i]);
      VkVP::Compute::graphicsMemoryBarrierRelease(
          storageBuffer, drawCmdBuffers[i], graphics_QFI, compute_QFI);
      VK_CHECK_RESULT(vkEndCommandBuffer(drawCmdBuffers[i]));
    }
    initializers::beginCommandBuffer(compute.commandBuffer);
    VkVP::Compute::computeMemoryBarrierAquire(
        storageBuffer, compute.commandBuffer, graphics_QFI, compute_QFI);
    buildComputeCommandBuffer(compute, storageBuffer, N_particles,
                              graphics_QFI);
    VkVP::Compute::computeMemoryBarrierRelease(
        storageBuffer, compute.commandBuffer, graphics_QFI, compute_QFI);
    vkEndCommandBuffer(compute.commandBuffer);
    VkVP::submitBuffers(vulkanInstance, compute.commandBuffer,
                        graphics.semaphore, compute.semaphore);
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