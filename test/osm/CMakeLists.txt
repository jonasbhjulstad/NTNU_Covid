
add_executable(osm_way osm_way.cpp)
target_link_libraries(osm_way PUBLIC ${OSM_XML_LIBRARIES} VulkanViewport)