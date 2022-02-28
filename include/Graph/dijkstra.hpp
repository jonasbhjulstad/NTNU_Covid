//
// Copyright 2021 Andrew Lumsdaine, Kevin Deweese
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Dijkstra, assuming graph stores vertex id and edge id
// 

#ifndef NWGRAPH_DIJKSTRA_HPP
#define NWGRAPH_DIJKSTRA_HPP

#include "graph_concepts.hpp"

#include <algorithm>
#include <concepts>
#include <limits>
#include <queue>
#include <ranges>
#include <tuple>
#include <vector>

template <adjacency_list Graph, std::invocable<inner_value_t<Graph>> WeightFunction =
                                    std::function<std::tuple_element_t<1, inner_value_t<Graph>>(const inner_value_t<Graph>&)>>
auto dijkstra(
    const Graph& graph, vertex_id_t<Graph> source, WeightFunction weights = [](const inner_value_t<Graph>& e) { return std::get<1>(e); }) {
  using vertex_id_type  = vertex_id_t<Graph>;
  using weight_type     = std::invoke_result_t<WeightFunction, inner_value_t<Graph>>;
  using weighted_vertex = std::tuple<vertex_id_type, weight_type>;

  std::vector<weight_type> distance(size(graph), std::numeric_limits<weight_type>::max());
  distance[source] = 0;

  std::priority_queue<weighted_vertex, std::vector<weighted_vertex>,
                      decltype([](auto&& a, auto&& b) { return (std::get<1>(a) > std::get<1>(b)); })>
      Q;
  Q.push({source, distance[source]});

  while (!Q.empty()) {
    auto u = std::get<0>(Q.top());
    Q.pop();

    for (auto&& e : graph[u]) {
      auto v = target(graph, e);                       // neighbor vertex
      if (distance[u] + weights(e) < distance[v]) {    // relax
        distance[v] = distance[u] + weights(e);
        Q.push({v, distance[v]});
      }
    }
  }
  return distance;
}

#endif    // NWGRAPH_DIJKSTRA_HPP
