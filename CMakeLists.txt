cmake_minimum_required(VERSION 3.20)

project(imgui-node-editor)

# Define IMGUI_NODE_EDITOR_ROOT_DIR pointing to project root directory
set(IMGUI_ROOT_DIR "/home/deb/Documents/Epidemiological_Model_Spread/include/imgui-docking")
set(CASADI_ROOT_DIR "/home/deb/Downloads/casadi/")

# Compile as static library 
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CASADI_ROOT_DIR}/cmake)


# Point CMake where to look for module files.


# Node editor use C++14
set(CMAKE_CXX_STANDARD            20)
set(CMAKE_CXX_STANDARD_REQUIRED   YES)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -DGLEW_STATIC")
set(CMAKE_CXX_FLAGS_DEBUG "-g -pg")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
add_executable(NetworkViewport main.cpp)
# find_package(imgui_node_editor REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(CASADI REQUIRED)
find_package(imgui REQUIRED)
# target_include_directories(NetworkViewport PRIVATE ${CASADI_INCLUDE})
target_link_libraries(NetworkViewport PRIVATE application GLEW::GLEW OpenGL::GL CASADI)
