

add_library(NV_glTFBasicInstance STATIC NV_glTFBasicInstance.cpp)
target_link_libraries(NV_glTFBasicInstance PUBLIC NV_VulkanInitializers NV_VulkanBuffer NV_VulkanglTFModel)
target_include_directories(NV_glTFBasicInstance PUBLIC ${CMAKE_SOURCE_DIR}/gltf ${CMAKE_SOURCE_DIR}/Vulkan)

add_library(NV_Assets STATIC NV_Assets.cpp)
target_link_libraries(NV_Assets PUBLIC Vulkan::Vulkan NV_VulkanglTFModel)
target_include_directories(NV_Assets PUBLIC "${CMAKE_SOURCE_DIR}/gltf/")