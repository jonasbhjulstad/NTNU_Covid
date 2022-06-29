cmake_minimum_required(VERSION 3.20)
project(NetworkViewport)
include(FetchContent)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(OpenMP_CXX "${CMAKE_CXX_COMPILER}" CACHE STRING "" FORCE)
  set(OpenMP_CXX_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument" CACHE STRING "" FORCE)
  set(OpenMP_CXX_LIB_NAMES "libomp" "libgomp" "libiomp5" CACHE STRING "" FORCE)
  set(OpenMP_libomp_LIBRARY ${OpenMP_CXX_LIB_NAMES} CACHE STRING "" FORCE)
  set(OpenMP_libgomp_LIBRARY ${OpenMP_CXX_LIB_NAMES} CACHE STRING "" FORCE)
  set(OpenMP_libiomp5_LIBRARY ${OpenMP_CXX_LIB_NAMES} CACHE STRING "" FORCE)
endif()

find_package(OpenMP REQUIRED)
find_package(Vulkan REQUIRED)
find_package(LAPACK REQUIRED)
find_package(igraph REQUIRED)
find_package(imgui REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Ktx REQUIRED)

target_link_libraries(igraph::igraph INTERFACE LAPACK::LAPACK)
find_package(VulkanTools REQUIRED)
target_link_libraries(VulkanTools::VulkanTools INTERFACE KTX::ktx)


FetchContent_Declare(randomCL_repo
GIT_REPOSITORY https://github.com/bstatcomp/RandomCL.git
)
FetchContent_MakeAvailable(randomCL_repo)
set(EXAMPLES_DATA_DIR "${CMAKE_BINARY_DIR}/examples/data/")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/examples/data")
set(CL_GENERATOR_DIR "${randomcl_repo_SOURCE_DIR}/generators")
set(CL_KERNEL_DIR "${CMAKE_SOURCE_DIR}/OpenCL/Kernels/")
set(CL_COMPILE_DEFINITIONS "-DRANDOM_CL_GENERATOR_DIR=${RANDOM_CL_GENERATOR_DIR} -DCL_TARGET_OPENCL_VERSION=300")

file(GLOB NV_HEADERS NetworkViewport/Graph/*.hpp NetworkViewport/ImGui/*.hpp NetworkViewport/Menu/*.hpp NetworkViewport/Utils/*.hpp)
file(GLOB NV_SOURCE NetworkViewport/Graph/*.cpp NetworkViewport/ImGui/*.cpp NetworkViewport/Menu/*.cpp)

add_library(NetworkViewport STATIC)
target_sources(NetworkViewport PRIVATE ${NV_SOURCE} PUBLIC FILE_SET HEADERS 
BASE_DIRS "${PROJECT_SOURCE_FOLDER}" FILES ${NV_HEADERS})
target_link_libraries(NetworkViewport PRIVATE imgui igraph::igraph KTX::ktx VulkanTools::VulkanTools)

add_subdirectory(Executables)