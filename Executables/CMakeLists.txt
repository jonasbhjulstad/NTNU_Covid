# add_executable(ER_Clusters ER_Clusters.cpp)
# # target_include_directories(main PUBLIC "${CMAKE_SOURCE_DIR}/Vulkan")
# target_link_libraries(ER_Clusters PUBLIC imgui "libktx.a" 
#                     Vulkan::Vulkan igraph::igraph glfw 
#                     glm::glm VulkanSetup Camera 
#                     ImGuiUI VulkanWindow VulkanInstance
#                     glTFBasicInstance ProjectionBuffer Graph)

add_executable(ER_Clusters_2D ER_Clusters_2D.cpp)
target_link_libraries(ER_Clusters_2D PUBLIC NetworkViewport)
get_cmake_property(_variableNames VARIABLES)
foreach(_variableName ${_variableNames})
  if(_variableName MATCHES "CLCPP.*LIBRARIES")
      message(STATUS "${_variableName}")
      message(STATUS "\t${${_variableName}}")
  endif()
endforeach()
# target_include_directories(main PUBLIC "${CMAKE_SOURCE_DIR}/Vulkan")
# target_link_libraries(ER_Clusters_2D PUBLIC LAPACK::LAPACK imgui igraph::igraph Vulkan::Vulkan glfw igraph::igraph
                    # glm::glm NetworkViewport KTX::ktx)