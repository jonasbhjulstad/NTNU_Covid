cmake_minimum_required(VERSION 3.20)
project(NetworkViewport)
include(FetchContent)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(FindOpenCL)
if(WIN32)
add_compile_definitions("VK_USE_PLATFORM_WIN32_KHR")
else()
add_compile_definitions("VK_USE_PLATFORM_XCB_KHR")
endif()

find_package(Vulkan REQUIRED)
find_package(igraph REQUIRED)
# find_package(XCB REQUIRED)
set(VULKAN_EXTERNAL_LIB_PATH "${CMAKE_SOURCE_DIR}/external/")
# file(GLOB IMGUI_SRC "${VULKAN_EXTERNAL_LIB_PATH}/imgui/*.cpp")
# add_library(imgui STATIC ${IMGUI_SRC})
# target_include_directories(imgui PUBLIC "${VULKAN_EXTERNAL_LIB_PATH}/imgui/" ${VULKAN_INCLUDE_DIRS})
# target_link_libraries(imgui PUBLIC Vulkan::Vulkan "-lvulkan")

FetchContent_Declare(
    imgui_repo
    GIT_REPOSITORY https://github.com/ocornut/imgui
)

FetchContent_MakeAvailable(imgui_repo)
file(GLOB IMGUI_SRC "${imgui_repo_SOURCE_DIR}/*.cpp")
file(GLOB IMGUI_BACKEND "${imgui_repo_SOURCE_DIR}/examples/imgui_impl_vulkan.cpp" "${imgui_repo_SOURCE_DIR}/examples/imgui_impl_glfw.cpp")
add_library(imgui STATIC ${IMGUI_SRC} ${IMGUI_BACKEND})
target_include_directories(imgui PUBLIC ${imgui_repo_SOURCE_DIR} "${imgui_repo_SOURCE_DIR}/backends" ${VULKAN_INCLUDE_DIRS})
target_link_libraries(imgui PUBLIC Vulkan::Vulkan "-lvulkan")



file(GLOB KTX_SRC "${VULKAN_EXTERNAL_LIB_PATH}/ktx/lib/*.cxx" "${VULKAN_EXTERNAL_LIB_PATH}/ktx/lib/*.c")
add_library(ktx ${KTX_SRC})
target_compile_definitions(ktx PUBLIC "KTX_OPENGL_ES3" "_DEBUG=0")
target_include_directories(ktx PUBLIC "${VULKAN_EXTERNAL_LIB_PATH}/ktx/include/" "${VULKAN_EXTERNAL_LIB_PATH}/ktx/other_include/")



set(TINYGLTF_INCLUDE "${VULKAN_EXTERNAL_LIB_PATH}/tinygltf/")


file(GLOB VULKAN_BASE_SRC "${CMAKE_SOURCE_DIR}/base/*.cpp")
add_library(Vulkan_Base STATIC ${VULKAN_BASE_SRC})
target_include_directories(Vulkan_Base PUBLIC "${CMAKE_SOURCE_DIR}/base/" ${TINYGLTF_INCLUDE} ${VULKAN_INCLUDE_DIRS})
target_link_libraries(Vulkan_Base PUBLIC ktx)

if (WIN32)

else()
find_package(X11 REQUIRED)
target_include_directories(Vulkan_Base PUBLIC "${CMAKE_SOURCE_DIR}/base/" ${TINYGLTF_INCLUDE} ${X11_INCLUDE_DIR} ${VULKAN_INCLUDE_DIRS})
target_link_libraries(Vulkan_Base PUBLIC ktx imgui ${X11_LIBRARIES} xcb Vulkan::Vulkan "-lvulkan")
endif()

set(VULKAN_BASE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/base/")


add_executable(main main.cpp)
target_include_directories(main PUBLIC ${VULKAN_BASE_INCLUDE_PATH} ${TINYGLTF_INCLUDE} ${VULKAN_INCLUDE_DIRS})
target_link_libraries(main PUBLIC imgui ktx Vulkan::Vulkan Vulkan_Base igraph::igraph)

# add_executable(SIR SIR.cpp)
# target_link_libraries(SIR PUBLIC imgui ktx Vulkan::Vulkan Vulkan_Base)
# target_include_directories(SIR PUBLIC ${VULKAN_BASE_INCLUDE_PATH} ${TINYGLTF_INCLUDE})

# add_subdirectory(examples)