# MACRO(SUBDIRLIST result curdir)
#     FILE(GLOB children ${curdir}/*) # This was changed
#     SET(dirlist "")
#     FOREACH(child ${children})
#         IF(IS_DIRECTORY ${child}) # This was changed
#             LIST(APPEND dirlist ${child})
#         ENDIF()
#     ENDFOREACH()
#     SET(${result} ${dirlist})
# ENDMACRO()

# add_library(NV_VulkanCommon INTERFACE)
# # SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
# set(SUBDIRS Buffer Debug Window Initializers Device SwapChain Instance PipelineInitializers Setup)
# FOREACH(subdir ${SUBDIRS})
# ADD_SUBDIRECTORY(${subdir})
# list(APPEND NV_VULKAN_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
# # get_filename_component(libname "${subdir}.cpp" NAME_WE)
# list(APPEND NV_VULKAN_LIBRARIES "NV_Vulkan${subdir}")
# ENDFOREACH()

# message(STATUS ${NV_VULKAN_INCLUDE_DIRECTORIES})
# foreach(LIB ${NV_VULKAN_LIBRARIES})
# target_link_libraries(${LIB} PRIVATE Vulkan::Vulkan)
# target_link_libraries(${LIB} INTERFACE NV_VulkanCommon)
# target_include_directories(${LIB} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" ${NV_VULKAN_INCLUDE_DIRECTORIES})
# endforeach()


file(GLOB NV_VULKAN_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(NV_Vulkan STATIC ${NV_VULKAN_SRC_FILES})
target_include_directories(NV_Vulkan PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(NV_Vulkan PRIVATE glfw3 imgui Vulkan::Vulkan)