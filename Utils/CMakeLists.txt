configure_file(NV_FilePaths.h.in NV_FilePaths.h)
add_library(NV_INCLUDE_PATHS INTERFACE)
target_include_directories(NV_INCLUDE_PATHS INTERFACE "${CMAKE_CURRENT_BINARY_DIR}")