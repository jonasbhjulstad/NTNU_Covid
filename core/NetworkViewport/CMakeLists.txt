file(GLOB LIB_SOURCE_FILES "./NV_*.cpp")

foreach(LIB_SRC ${LIB_SOURCE_FILES})
    get_filename_component(FILE_NAME ${LIB_SRC} NAME_WE)
    add_library(${FILE_NAME} STATIC ${LIB_SRC})
    target_link_libraries(${FILE_NAME} PRIVATE Vulkan::Vulkan glfw)
    message(WARNING ${LIB_SRC})
    list(APPEND ${NV_LIBS} ${LIB_SRC})
endforeach(LIB_SRC)

message(STATUS ${NV_LIBS})

target_link_libraries(NV_SwapChain PRIVATE NV_Queue)
target_link_libraries(NV_GraphicsPipeline PRIVATE NV_Queue NV_ShaderModule NV_SwapChain NV_Buffers)
target_link_libraries(NV_Command PRIVATE NV_Queue)
target_link_libraries(NV_Application PRIVATE NV_SwapChain NV_GraphicsPipeline NV_Command NV_Synchronization NV_Buffers)