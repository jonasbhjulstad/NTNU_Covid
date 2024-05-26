#find program glslc
find_program(GLSLC glslc)

file(GLOB FRAG_SHADERS *.frag)
file(GLOB VERT_SHADERS *.vert)



foreach(frag ${FRAG_SHADERS})
    get_filename_component(frag_name ${frag} NAME_WE)
    set(compiled_name ${VulkanViewport_SHADER_DIR}/${frag_name}.frag.spv)
    add_custom_command(OUTPUT ${compiled_name}
        COMMAND ${GLSLC} ${frag} -o ${compiled_name}
        DEPENDS ${frag}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_custom_target(${frag_name}_fragment_shader ALL
        DEPENDS ${VulkanViewport_SHADER_DIR}/${frag_name}.frag.spv)
    list(APPEND FRAG_SPV ${CMAKE_CURRENT_BINARY_DIR}/${frag_name}.spv)
endforeach()

foreach(vert ${VERT_SHADERS})
    get_filename_component(vert_name ${vert} NAME_WE)
    set(compiled_name ${VulkanViewport_SHADER_DIR}/${vert_name}.vert.spv)
    add_custom_command(
        OUTPUT ${compiled_name}
        COMMAND ${GLSLC} ${vert} -o ${compiled_name}
        DEPENDS ${vert}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    add_custom_target(${vert_name}_vertex_shader ALL
        DEPENDS ${compiled_name})
    list(APPEND VERT_SPV ${CMAKE_CURRENT_BINARY_DIR}/${vert_name}.spv)
endforeach()

set(GRAPH_SHADERS node_fragment_shader node_vertex_shader edge_fragment_shader edge_vertex_shader PARENT_SCOPE)
set(VOXEL_SHADERS cube_fragment_shader cube_vertex_shader PARENT_SCOPE)
set(WAY_SHADERS way_fragment_shader way_vertex_shader PARENT_SCOPE)

set(UI_SHADERS ui_fragment_shader ui_vertex_shader PARENT_SCOPE)