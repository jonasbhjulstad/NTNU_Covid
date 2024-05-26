#define KTX_OPENGL_ES3 1
#define ENABLE_VALIDATION true
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <VulkanTools/Camera.hpp>
#include <VulkanTools/ProjectionBuffer.hpp>
#include <VulkanTools/Setup.hpp>
#include <VulkanTools/Window.hpp>
#include <VulkanTools/gltf/glTFModel.hpp>
#include <VulkanViewport/Filepaths.hpp>
#include <VulkanViewport/Graph_Layout.hpp>
#include <VulkanViewport/ImGuiUI.hpp>
#include <VulkanViewport/SetupRoutines.hpp>
#include <VulkanViewport/UISettings.hpp>
#include <VulkanViewport/osm/WayHandler.hpp>
#include <chrono>
#include <imgui/imgui.h>
#include <memory>
#include <random>
#include <vulkan/vulkan.hpp>

std::vector<VkVP::WayInstanceData>
way_instance_from_osm(const std::string &filepath) {

  std::vector<VkVP::WayInstanceData> data;
  osmium::io::File file(filepath);
  osmium::io::Reader reader{file};

  namespace map = osmium::index::map;
  using index_type =
      map::SparseMemArray<osmium::unsigned_object_id_type, osmium::Location>;
  using location_handler_type =
      osmium::handler::NodeLocationsForWays<index_type>;

  index_type index;
  location_handler_type location_handler{index};

  // get number of nodes in the file
  osmium::io::Reader reader2{file};

  std::vector<VkVP::WayInstanceData> way_data;
  VkVP::WayHandler handler(way_data);
  osmium::apply(reader, location_handler, handler);
  return way_data;
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
      glfwCreateWindow(width, height, "WayCubes", NULL, NULL);

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

  createVulkanInstance(ENABLE_VALIDATION, "WayCubes", vulkanInstance.instance,
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
  auto wayInstanceData = way_instance_from_osm("/tmp/pyrosm/Trondheim.osm.pbf");
  InstanceRenderingParams wayParams;
  wayParams.vertexShaderPath = SHADER_DIR + "way.vert.spv";
  wayParams.fragmentShaderPath = SHADER_DIR + "way.frag.spv";
  wayParams.modelPath = MODEL_DIR + "cube.gltf";
  wayParams.vulkanDevice = vulkanInstance.vulkanDevice;
  wayParams.uniformProjectionBuffer = &vulkanInstance.projection.buffer;
  wayParams.queue = vulkanInstance.queue;
  wayParams.renderPass = vulkanInstance.renderPass;
  wayParams.pipelineCache = vulkanInstance.pipelineCache;
  wayParams.descriptorPool = renderDescriptorPool;
  wayParams.offset = offset;

  std::vector<std::unique_ptr<InstancePipelineData>> instancePipelines;
  instancePipelines.push_back(
      prepareInstanceRendering<WayInstanceData>(wayParams, wayInstanceData));

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
                                       vulkanInstance.queue, SHADER_DIR);

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

    VkVP::newFrame(uiSettings, frameTimer, camera);

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