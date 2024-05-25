include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)
include(FindVulkan)


# CPMFindPackage(NAME VulkanTools
# GITHUB_REPOSITORY jonasbhjulstad/VulkanTools)
find_package(glfw3 REQUIRED)
find_package(VulkanTools REQUIRED)


CPMFindPackage(NAME imgui
GITHUB_REPOSITORY jonasbhjulstad/imgui)


find_package(OpenMP REQUIRED)
find_package(igraph REQUIRED)
find_package(glm REQUIRED)
find_package(Ktx REQUIRED)
find_package(LAPACK REQUIRED)