file(GLOB ParticleCompute_src *.cpp)
add_executable(ParticleCompute ${ParticleCompute_src})
target_link_libraries(ParticleCompute PUBLIC ComputeParticleLib)
# add_dependencies(VoxelGround ${CUBE_COMPUTE_SHADERS})