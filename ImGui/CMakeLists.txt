add_library(NV_ImGuiUI STATIC NV_ImGuiUI.cpp)
target_link_libraries(NV_ImGuiUI PUBLIC imgui glfw3 Vulkan::Vulkan NV_VulkanBuffer NV_VulkanDevice NV_VulkanInitializers NV_VulkanPipelineInitializers)
target_include_directories(NV_ImGuiUI PUBLIC ${CMAKE_SOURCE_DIR}/Menu/)
target_include_directories(NV_ImGuiUI PUBLIC ${CMAKE_SOURCE_DIR}/gltf ${CMAKE_SOURCE_DIR}/Vulkan)