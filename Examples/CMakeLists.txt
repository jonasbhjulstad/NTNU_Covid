find_package(OpenCL REQUIRED)
add_executable(RNG_Example RNG_Example.cpp)
target_link_libraries(RNG_Example PUBLIC OpenCL NV_INCLUDE_PATHS)
add_definitions(${CL_COMPILE_DEFINITIONS} -D PWD=${CMAKE_CURRENT_SOURCE_DIR}/)
target_include_directories(RNG_Example PUBLIC ${NV_UTILS_DIR} "../Utils")
target_link_libraries(RNG_Example PUBLIC NV_CL_DefaultInstance)

add_subdirectory(SIR)
add_subdirectory(Network)