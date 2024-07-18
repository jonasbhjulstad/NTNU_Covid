#find program glslc
find_program(GLSLC glslc)

function(add_shaders extension_name)
file(GLOB shader_files *.${extension_name})

foreach(shader ${shader_files})
get_filename_component(shader_name ${shader} NAME_WE)
#get dir of shader
get_filename_component(shader_dir ${shader} DIRECTORY)

file(RELATIVE_PATH rel_dir ${PROJECT_SOURCE_DIR} ${shader})
#get dir of shader
get_filename_component(rel_dir ${rel_dir} DIRECTORY)


set(compiled_name ${PROJECT_BINARY_DIR}/${rel_dir}/${shader_name}.${extension_name}.spv)
add_custom_command(OUTPUT ${compiled_name}
    COMMAND ${GLSLC} ${shader} -o ${compiled_name}
    DEPENDS ${shader}
    WORKING_DIRECTORY ${shader_dir}
)
add_custom_target(${shader_name}_${extension_name}_shader ALL
    DEPENDS ${compiled_name})
endforeach()
endfunction()


add_shaders("vert")
add_shaders("frag")
add_subdirectory(ComputeCube)

set(GRAPH_SHADERS node_frag_shader node_vert_shader edge_frag_shader edge_vert_shader PARENT_SCOPE)
set(VOXEL_SHADERS cube_frag_shader cube_vert_shader PARENT_SCOPE)
set(WAY_SHADERS way_frag_shader way_vert_shader PARENT_SCOPE)

set(UI_SHADERS ui_frag_shader ui_vert_shader PARENT_SCOPE)