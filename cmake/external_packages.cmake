include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)


CPMFindPackage(NAME VulkanTools
GITHUB_REPOSITORY jonasbhjulstad/CLCPP-Vulkan-Tools)

CPMFindPackage(NAME imgui
GITHUB_REPOSITORY jonasbhjulstad/imgui)