#pragma once
#include <VulkanViewport/Instance/Way.hpp>
#include <osmium/handler.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>
#include <osmium/index/map/sparse_mem_array.hpp>
#include <osmium/io/any_input.hpp>
#include <osmium/osm/node.hpp>
#include <osmium/osm/way.hpp>
#include <osmium/visitor.hpp>

namespace VkVP {
class WayHandler : public osmium::handler::Handler {
public:
  WayHandler(std::vector<VkVP::WayInstanceData> &way_data)
      : way_data(way_data) {}
  std::vector<VkVP::WayInstanceData> &way_data;
   void way(const osmium::Way &way);
};
} // namespace VkVP