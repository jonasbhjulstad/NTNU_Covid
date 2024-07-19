include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)
include(FindVulkan)


find_package(glfw3 REQUIRED)

CPMAddPackage("gh:jonasbhjulstad/VulkanTools#v1.0.9")
CPMAddPackage("gh:jonasbhjulstad/imgui#v1.0.2")

# find_package(OpenMP REQUIRED)
# find_package(igraph REQUIRED)
find_package(glm REQUIRED)
find_package(Ktx REQUIRED)
# find_package(LAPACK REQUIRED)

# find_package(expat REQUIRED)
# find_package(ZLIB REQUIRED)
# include(FindBZip2)
# find_package(Osmium REQUIRED)
# set(OSM_XML_LIBRARIES  ZLIB::ZLIB BZip2::BZip2 expat::expat ${OSMIUM_XML_LIBRARIES})