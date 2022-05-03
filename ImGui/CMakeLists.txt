add_library(NV_ImGuiUI STATIC NV_ImGuiUI.cpp)
add_library(NV_Graph_Designer STATIC NV_Graph_Designer.cpp)
add_library(NV_Menu STATIC NV_Menu.cpp)

target_link_libraries(NV_Graph_Designer PUBLIC imgui igraph::igraph)
target_link_libraries(NV_Menu PUBLIC imgui NV_Graph_Designer )

target_link_libraries(NV_ImGuiUI PUBLIC imgui glfw3 Vulkan::Vulkan NV_VulkanBuffer NV_VulkanDevice NV_VulkanPipelineInitializers NV_Menu NV_Assets)
target_include_directories(NV_ImGuiUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})