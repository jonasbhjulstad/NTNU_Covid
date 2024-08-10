file(GLOB VertexLighting_src *.cpp)
add_executable(VertexLighting ${VertexLighting_src})
target_link_libraries(VertexLighting PUBLIC VulkanViewport)