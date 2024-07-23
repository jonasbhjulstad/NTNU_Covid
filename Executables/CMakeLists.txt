add_executable(VoxelGround VoxelGround.cpp)
target_link_libraries(VoxelGround PUBLIC VulkanViewport)
add_dependencies(VoxelGround ${VOXEL_SHADERS})

add_subdirectory(compute_cube)
add_subdirectory(vertex_lighting)
add_subdirectory(particle_compute)

