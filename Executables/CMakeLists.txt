# add_executable(ER_Clusters ER_Clusters.cpp)
# # target_include_directories(main PUBLIC "${CMAKE_SOURCE_DIR}/Vulkan")
# target_link_libraries(ER_Clusters PUBLIC imgui "libktx.a" 
                    # Vulkan::Vulkan igraph::igraph glfw 
                    # glm::glm VulkanSetup Camera 
                    # UI/ImGuiUI VulkanWindow VulkanInstance
                    # glTFBasicInstance ProjectionBuffer Graph)


# add_executable(WayCubes WayCubes.cpp)
# target_link_libraries(WayCubes PUBLIC VulkanViewport)
# add_dependencies(WayCubes ${WAY_SHADERS})


add_executable(VoxelGround VoxelGround.cpp)
target_link_libraries(VoxelGround PUBLIC VulkanViewport)
add_dependencies(VoxelGround ${VOXEL_SHADERS})

add_subdirectory(computecube)


# target_include_directories(main PUBLIC "${CMAKE_SOURCE_DIR}/Vulkan")
# target_link_libraries(ER_Clusters_2D PUBLIC LAPACK::LAPACK imgui igraph::igraph Vulkan::Vulkan glfw igraph::igraph
                    # glm::glm VulkanViewport KTX::ktx)