#include <VulkanViewport/osm/WayHandler.hpp>
namespace VkVP
{
  void WayHandler::way(const osmium::Way &way) {
    way_data.reserve(way.nodes().size());
    bool first = true;
    auto prev_node = way.nodes()[0];
    for (const auto &n : way.nodes()) {
      VkVP::WayInstanceData data;
      // pos is in the middle of the two nodes
      data.pos =
          glm::vec2((prev_node.location().lon() + n.location().lon()) / 2.0f,
                    (prev_node.location().lat() + n.location().lat()) / 2.0f);
      // scale is the distance between the two nodes hyp
      data.scale =
          sqrt(pow(prev_node.location().lon() - n.location().lon(), 2) +
               pow(prev_node.location().lat() - n.location().lat(), 2) / 2.0f);
      // north is 0 rad
      data.rotation = atan2(n.location().lat() - prev_node.location().lat(),
                            n.location().lon() - prev_node.location().lon());
      way_data.push_back(data);
      prev_node = n;
      if (first) {
        first = false;
        prev_node = n;
        continue;
      }
    }
  }
}