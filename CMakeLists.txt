cmake_minimum_required(VERSION 3.20)
project(NetworkViewport)
include(FetchContent)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(FindOpenCL)

find_package(Vulkan REQUIRED)
find_package(igraph REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(kompute REQUIRED)
find_package(glm REQUIRED)
FetchContent_Declare(
    imgui_repo
    GIT_REPOSITORY https://github.com/ocornut/imgui
)
find_package(fmt REQUIRED)
set(CMAKE_BUILD_PARALLEL_LEVEL 8)
FetchContent_MakeAvailable(imgui_repo)
file(GLOB IMGUI_SRC "${imgui_repo_SOURCE_DIR}/*.cpp")
set(IMGUI_BACKEND "${imgui_repo_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp" "${imgui_repo_SOURCE_DIR}/backends/imgui_impl_glfw.cpp")
add_library(imgui STATIC ${IMGUI_SRC} ${IMGUI_BACKEND})
target_include_directories(imgui PUBLIC ${imgui_repo_SOURCE_DIR} "${imgui_repo_SOURCE_DIR}/backends")
target_link_libraries(imgui PUBLIC Vulkan::Vulkan)
target_compile_options(imgui PRIVATE "-O3")

find_package(Ktx REQUIRED)
# if (NOT ${ktx_FOUND})
#     FetchContent_Declare(
#         ktx_repo
#         GIT_REPOSITORY https://github.com/KhronosGroup/KTX-Software.git
#     )
#     option(KTX_FEATURE_TESTS OFF)
#     FetchContent_MakeAvailable(ktx_repo)
#     target_compile_definitions(ktx PUBLIC "KTX_OPENGL_ES3")
#     target_compile_options(ktx PRIVATE "-O3")
#     endif()
set(NV_VULKAN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan)
set(NV_GLTF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gltf)
set(NV_IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ImGui)
file(GLOB VULKAN_CXX_HEADER_FILES "${Vulkan_INCLUDE_DIRS}/vulkan/*.hpp")

# target_precompile_headers(Vulkan::Vulkan INTERFACE ${VULKAN_CXX_HEADER_FILES})

add_subdirectory(Vulkan)
add_subdirectory(ImGui)
add_executable(main main.cpp)
# target_include_directories(main PUBLIC "${CMAKE_SOURCE_DIR}/Vulkan")
message(WARNING NV_VulkanDevice)
target_link_libraries(main PUBLIC imgui "libktx.a" Vulkan::Vulkan igraph::igraph glfw glm::glm NV_VulkanSetup NV_Camera NV_ImGuiUI NV_VulkanWindow NV_VulkanInstance)
add_compile_options("-Wfatal-errors")
# add_subdirectory(examples)