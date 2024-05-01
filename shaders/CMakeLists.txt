#find program glslc
find_program(GLSLC glslc)

file(GLOB FRAG_SHADERS *.frag)
file(GLOB VERT_SHADERS *.vert)



foreach(frag ${FRAG_SHADERS})
    get_filename_component(frag_name ${frag} NAME_WE)
    add_custom_target(${frag_name}.frag.spv
        COMMAND ${GLSLC} ${frag} -o ${VulkanViewport_SHADER_DIR}/${frag_name}.frag.spv
        DEPENDS ${frag}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Compiling ${frag_name}.frag to ${frag_name}.frag.spv"
        VERBATIM
    )
    list(APPEND FRAG_SPV ${CMAKE_CURRENT_BINARY_DIR}/${frag_name}.spv)
endforeach()

foreach(vert ${VERT_SHADERS})
    get_filename_component(vert_name ${vert} NAME_WE)
    add_custom_target(${vert_name}.vert.spv
        COMMAND ${GLSLC} ${vert} -o ${VulkanViewport_SHADER_DIR}/${vert_name}.vert.spv
        DEPENDS ${vert}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Compiling ${vert_name}.vert to ${vert_name}.vert.spv"
        VERBATIM
    )
    list(APPEND VERT_SPV ${CMAKE_CURRENT_BINARY_DIR}/${vert_name}.spv)
endforeach()

set(GRAPH_SHADERS node.frag.spv node.vert.spv edge.frag.spv edge.vert.spv PARENT_SCOPE)
set(VOXEL_SHADERS cube.frag.spv cube.vert.spv PARENT_SCOPE)

set(UI_SHADERS ui.frag.spv ui.vert.spv PARENT_SCOPE)