cmake_minimum_required(VERSION 3.20)
# set(casadi_DIR "${casadi_repo_SOURCE_DIR}/cmake/")
find_package(casadi REQUIRED)
add_executable(OptSpringLayout OptSpringLayout.cpp)
target_link_libraries(OptSpringLayout PRIVATE casadi)

