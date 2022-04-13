cmake_minimum_required(VERSION 3.21)

project (test)

set(CMAKE_CXX_COMPILER dpcpp)

set(SYCL_INCLUDE_DIR /opt/intel/oneapi/compiler/latest/linux/include)
set(SYCL_LIBRARY_DIR /opt/intel/oneapi/compiler/latest/linux/lib)

find_package(IntelDPCPP REQUIRED)
find_package(SUNDIALS REQUIRED)
add_compile_options("-fsycl")
add_executable(syclTest cvAdvDiff_kry_sycl.cpp)
target_link_libraries(syclTest PUBLIC SUNDIALS::cvode SUNDIALS::nvecsycl)

include_directories("${SYCL_INCLUDE_DIR}" ${SUNDIALS_INCLUDE_DIR})

add_executable(SIR SIR.cpp)
target_link_libraries(SIR PUBLIC SUNDIALS::cvode SUNDIALS::nvecsycl)

add_executable(SIR_serial SIR_serial.cpp)
target_link_libraries(SIR_serial PUBLIC SUNDIALS::cvode SUNDIALS::nvecserial)

add_executable(SIR_sycl SIR_sycl.cpp)
target_link_libraries(SIR_sycl PUBLIC SUNDIALS::cvode SUNDIALS::nvecsycl)

add_executable(deviceinfo_sycl deviceinfo_sycl.cpp)