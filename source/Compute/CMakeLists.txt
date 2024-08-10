
file(GLOB COMPUTE_COMMON_LIB_SRC Common/*.cpp)
add_library(ComputeCommonLib ${COMPUTE_COMMON_LIB_SRC})
target_link_libraries(ComputeCommonLib PUBLIC VulkanViewport)


file(GLOB COMPUTE_CUBE_LIB_SRC Cube/*.cpp)
add_library(ComputeCubeLib ${COMPUTE_CUBE_LIB_SRC})
target_link_libraries(ComputeCubeLib PUBLIC VulkanViewport ComputeCommonLib)

file(GLOB COMPUTE_PARTICLE_LIB_SRC Particle/*.cpp)
add_library(ComputeParticleLib ${COMPUTE_PARTICLE_LIB_SRC})
target_link_libraries(ComputeParticleLib PUBLIC VulkanViewport ComputeCommonLib)

