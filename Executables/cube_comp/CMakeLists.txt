file(GLOB CubeComp_src *.cpp)
add_executable(CubeComp ${CubeComp_src})
target_link_libraries(CubeComp PUBLIC ComputeCubeLib)
# add_dependencies(VoxelGround ${CUBE_COMPUTE_SHADERS})