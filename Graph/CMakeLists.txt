add_library(NV_Graph INTERFACE)
target_include_directories(NV_Graph INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(NV_Graph INTERFACE NV_VulkanInitializers)