#ifndef NV_GRAPH_DESIGNER_HPP
#define NV_GRAPH_DESIGNER_HPP
#include <algorithm>
#include <memory>
#include <limits>
#include <imgui.h>
#include <igraph/igraph.h>
#include <igraph/igraph_games.h>
#define GRAPH_CREATION_MAX_NODES 1000
#define GRAPH_CREATION_MAX_EDGES 10000

enum GraphDesignStatus {GRAPH_DESIGN_STATUS_IDLE, GRAPH_DESIGN_STATUS_CANCELED,
GRAPH_DESIGN_STATUS_GRAPH_CREATED};
GraphDesignStatus createGraphDesignerMenu(igraph_t* graph);

#endif