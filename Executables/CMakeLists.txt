# add_executable(ER_Clusters ER_Clusters.cpp)
# # target_include_directories(main PUBLIC "${CMAKE_SOURCE_DIR}/Vulkan")
# target_link_libraries(ER_Clusters PUBLIC imgui "libktx.a" 
#                     Vulkan::Vulkan igraph::igraph glfw 
#                     glm::glm NV_VulkanSetup NV_Camera 
#                     NV_ImGuiUI NV_VulkanWindow NV_VulkanInstance
#                     NV_glTFBasicInstance NV_ProjectionBuffer NV_Graph)

add_executable(ER_Clusters_2D ER_Clusters_2D.cpp)
# target_include_directories(main PUBLIC "${CMAKE_SOURCE_DIR}/Vulkan")
target_link_libraries(ER_Clusters_2D PUBLIC LAPACK::LAPACK imgui Vulkan::Vulkan glfw igraph::igraph
                    glm::glm NV_VulkanSetup NV_Camera 
                    NV_ImGuiUI NV_VulkanWindow NV_VulkanInstance
                    NV_glTFBasicInstance NV_ProjectionBuffer NV_Graph ktx)