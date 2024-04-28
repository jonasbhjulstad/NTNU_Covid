#ifndef GRAPH_DESIGNER_HPP
#define GRAPH_DESIGNER_HPP
#include <igraph/igraph.h>
#include <imgui/imgui.h>
#include <string>
#include <NetworkViewport/Graph_Generation.hpp>
#define GRAPH_CREATION_MAX_NODES 1000
#define GRAPH_CREATION_MAX_EDGES 10000

namespace Menu
{

enum GraphDesignPage
{
    GRAPH_DESIGN_GENERATION,
    GRAPH_DESIGN_LAYOUT
};
struct GraphGenerationParam
{
    const char* graphType = "Erdös-Rényi";
    const char* ERType = "GNP";
    float p = .5;
    int N_nodes = 1;
    int N_edges = 0;
    float power = 1.f;
    float A = 1.f;
    int m = 1;
    int dim = 2;
    int size = 5;
    int neigborhoodSize = 2;
    float rewireProbability = .0;
    bool loops = 0;
    bool multipleEdges = 0;
};
struct GraphLayoutParam
{
    const char *layoutType = "Kamada-Kawai";
};
enum GraphDesignStatus {GRAPH_DESIGN_STATUS_IDLE, GRAPH_DESIGN_STATUS_CANCELED,
GRAPH_DESIGN_STATUS_GRAPH_CREATED, GRAPH_DESIGN_STATUS_NEXT};

GraphDesignStatus createGraphDesignerMenu(igraph_t* graph);
GraphDesignStatus displayGraphGeneration(igraph_t* graph, GraphGenerationParam& param);
GraphDesignStatus displayGraphLayout(igraph_t* graph, GraphLayoutParam& param);

}
#endif