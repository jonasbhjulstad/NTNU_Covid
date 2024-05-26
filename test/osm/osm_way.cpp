#include <osmium/handler.hpp>
#include <osmium/osm/node.hpp>
#include <osmium/osm/way.hpp>
#include <osmium/io/any_input.hpp>
#include <osmium/visitor.hpp>
#include <osmium/index/map/sparse_mem_array.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>
#include <VulkanViewport/Instance/Way.hpp>
#include <VulkanViewport/Filepaths.hpp>
#include <VulkanViewport/osm/WayHandler.hpp>
#include <string>
#include <iostream>
#include <algorithm>

int main()
{
    // Open a file for reading
    std::string filename = "";
    osmium::io::File file("/tmp/pyrosm/Trondheim.osm.pbf");
    osmium::io::Reader reader{file};


    namespace map = osmium::index::map;
    using index_type = map::SparseMemArray<osmium::unsigned_object_id_type, osmium::Location>;
    using location_handler_type = osmium::handler::NodeLocationsForWays<index_type>;

    index_type index;
    location_handler_type location_handler{index};

    //get number of nodes in the file
    osmium::io::Reader reader2{file};


    std::vector<VkVP::WayInstanceData> way_data;
    VkVP::WayHandler handler(way_data);
    osmium::apply(reader, location_handler, handler);
    reader.close();

    VkVP::WayInstanceData::write_csv(VkVP::OSM_MAP_DIR + "/test_osm_way.csv", way_data);
    
}