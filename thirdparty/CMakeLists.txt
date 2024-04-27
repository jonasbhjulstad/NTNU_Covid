file(GLOB IMGUI_SOURCES imgui/*.cpp)
file(GLOB IMGUI_HEADERS imgui/*.h)
file(GLOB IMGUI_BACKEND imgui/backends/*vulkan*)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS} ${IMGUI_BACKEND})
target_link_libraries(imgui PUBLIC Vulkan::Vulkan glfw)