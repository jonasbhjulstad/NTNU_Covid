file(GLOB ComputeCube_src *.cpp)
add_executable(ComputeCube ${ComputeCube_src})
target_link_libraries(ComputeCube PUBLIC ComputeCubeLib)
# add_dependencies(VoxelGround ${CUBE_COMPUTE_SHADERS})