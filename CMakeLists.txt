cmake_minimum_required(VERSION 3.29)
project(VulkanViewport)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
include(cmake/ccache.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/external_packages.cmake)


set(VulkanViewport_SHADER_DIR ${PROJECT_BINARY_DIR}/shaders/)
file(MAKE_DIRECTORY ${VulkanViewport_SHADER_DIR})
add_subdirectory(shaders)


set(VulkanViewport_SHADER_DIR ${PROJECT_BINARY_DIR}/shaders/)
set(VulkanViewport_ASSET_DIR ${PROJECT_SOURCE_DIR}/assets/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include/VulkanViewport)
configure_file(include/VulkanViewport/Filepaths.hpp.in ${PROJECT_BINARY_DIR}/include/VulkanViewport/Filepaths.hpp @ONLY)


file(GLOB VulkanViewport_SOURCES source/*.cpp)
add_library(VulkanViewport ${VulkanViewport_SOURCES})
add_subdirectory(source)
target_link_libraries(VulkanViewport PUBLIC imgui KTX::ktx VulkanTools::VulkanTools imgui::imgui)
target_include_directories(VulkanViewport PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
target_include_directories(VulkanViewport PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include> $<INSTALL_INTERFACE:include>)
add_dependencies(VulkanViewport ${UI_SHADERS})
add_subdirectory(Executables)
add_subdirectory(test)