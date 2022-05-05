MACRO(GENERATE_LIBRARIES DIRNAME)
set(NV_${DIRNAME}_LIBRARIES "")
file(GLOB NV_VULKAN_${DIRNAME}_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${DIRNAME}/*.cpp)
file(GLOB NV_VULKAN_${DIRNAME}_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
foreach(NV_VULKAN_SRC ${NV_VULKAN_${DIRNAME}_SRC_FILES})
get_filename_component(libname "${NV_VULKAN_SRC}" NAME_WE)
add_library(${libname} STATIC ${NV_VULKAN_SRC})
target_link_libraries(${libname} PUBLIC Vulkan::Vulkan glfw3 imgui)
target_include_directories(${libname} PUBLIC ${NV_VULKAN_DIR}/${DIRNAME})
list(APPEND NV_${DIRNAME}_LIBRARIES ${libname})
endforeach()
endmacro()

SUBDIRLIST(NV_VULKAN_SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

foreach(SUBDIR ${NV_VULKAN_SUBDIRS})
GENERATE_LIBRARIES(${SUBDIR})
endforeach()

foreach(SUBDIR ${NV_VULKAN_SUBDIRS})
add_subdirectory(${SUBDIR})
endforeach()





