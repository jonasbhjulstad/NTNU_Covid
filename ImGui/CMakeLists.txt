add_library(NV_ImGuiUI STATIC NV_ImGuiUI.cpp)

target_link_libraries(NV_ImGuiUI PUBLIC imgui glfw3 Vulkan::Vulkan NV_VulkanBuffer NV_VulkanPipelineInitializers NV_Menu NV_Assets NV_Camera)
target_include_directories(NV_ImGuiUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})