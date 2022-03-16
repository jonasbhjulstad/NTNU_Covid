cmake_minimum_required(VERSION 3.20)
project(Custom_Vulkan)
include(FetchContent)
set(MINGW_INCLUDE "C:\\msys64\\mingw64\\include")
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED HINTS "C:\\msys64\\mingw64\\lib\\cmake")


FetchContent_Declare(
    imgui_repo
    GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
)

set(CMAKE_CXX_STANDARD_REQUIRED 17)

FetchContent_MakeAvailable(imgui_repo)
file(GLOB IMGUI_SOURCE_FILES
    "${imgui_repo_SOURCE_DIR}/*.cpp"
    )
file(GLOB IMGUI_BACKEND_FILES
    "${imgui_repo_SOURCE_DIR}/backends/*glfw.cpp"
    "${imgui_repo_SOURCE_DIR}/backends/*vulkan.cpp"
    )
add_library(ImGui STATIC ${IMGUI_SOURCE_FILES} ${IMGUI_BACKEND_FILES})
target_include_directories(ImGui PUBLIC ${imgui_repo_SOURCE_DIR} ${imgui_repo_SOURCE_DIR}/backends)
target_link_libraries(ImGui PRIVATE Vulkan::Vulkan glfw)

add_subdirectory(core)
add_subdirectory(shaders)
add_executable(AppTest ImguiMain.cpp)
target_link_libraries(AppTest PUBLIC Vulkan::Vulkan glfw ImGui ${NV_LIBS})
include_directories(${MINGW_INCLUDE})

if (${Win32})
  if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
  else()
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin32/glslangValidator.exe")
  endif()
else()
    set(GLSL_VALIDATOR /usr/bin/glslc)
endif()

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "shaders/*.frag"
    "shaders/*.vert"
    )

foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
    COMMAND ${GLSL_VALIDATOR} ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders 
    DEPENDS ${SPIRV_BINARY_FILES}
    )

add_dependencies(AppTest Shaders)

add_custom_command(TARGET AppTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:AppTest>/shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_BINARY_DIR}/shaders"
        "$<TARGET_FILE_DIR:AppTest>/shaders"
        )