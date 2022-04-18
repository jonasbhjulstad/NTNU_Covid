cmake_minimum_required(VERSION 3.20)
project(NetworkViewport)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(FindOpenCL)
add_compile_definitions("VK_USE_PLATFORM_XCB_KHR")


find_package(Vulkan REQUIRED)
# find_package(XCB REQUIRED)
set(VULKAN_EXTERNAL_LIB_PATH "../../external/")
file(GLOB IMGUI_SRC "${VULKAN_EXTERNAL_LIB_PATH}/imgui/*.cpp")
add_library(imgui ${IMGUI_SRC})
target_include_directories(imgui PUBLIC "${VULKAN_EXTERNAL_LIB_PATH}/imgui/")

file(GLOB KTX_SRC "${VULKAN_EXTERNAL_LIB_PATH}/ktx/lib/*.cxx" "${VULKAN_EXTERNAL_LIB_PATH}/ktx/lib/*.c")
add_library(ktx ${KTX_SRC})
target_compile_definitions(ktx PRIVATE "KTX_OPENGL_ES3" "_DEBUG=0")
target_include_directories(ktx PUBLIC "${VULKAN_EXTERNAL_LIB_PATH}/ktx/include/" "${VULKAN_EXTERNAL_LIB_PATH}/ktx/other_include/")



set(TINYGLTF_INCLUDE "${VULKAN_EXTERNAL_LIB_PATH}/tinygltf/")



file(GLOB VULKAN_BASE_SRC "../../base/*.cpp")
add_library(Vulkan_Base ${VULKAN_BASE_SRC})

if (WIN32)

else()
find_package(X11 REQUIRED)
target_include_directories(Vulkan_Base PUBLIC "../../base/" ${TINYGLTF_INCLUDE} ${X11_INCLUDE_DIR})
target_link_libraries(Vulkan_Base PUBLIC ktx imgui ${X11_LIBRARIES} xcb)
endif()

set(VULKAN_BASE_INCLUDE_PATH "../../base/")


add_executable(main main.cpp)
target_link_libraries(main PUBLIC imgui ktx Vulkan::Vulkan Vulkan_Base)
target_include_directories(main PUBLIC ${VULKAN_BASE_INCLUDE_PATH} ${TINYGLTF_INCLUDE})

add_executable(SIR SIR.cpp)
target_link_libraries(SIR PUBLIC imgui ktx Vulkan::Vulkan Vulkan_Base)
target_include_directories(SIR PUBLIC ${VULKAN_BASE_INCLUDE_PATH} ${TINYGLTF_INCLUDE})

add_subdirectory(examples)