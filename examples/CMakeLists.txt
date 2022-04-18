function(add_cl_example subdir)
add_executable(${subdir} "${subdir}/${subdir}.cpp")
target_link_libraries(${subdir} PUBLIC OpenCL::OpenCL)
endfunction()

add_cl_example(cl_vector_add)