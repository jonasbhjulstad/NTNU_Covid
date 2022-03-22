#ifndef NV_FILE_UTILS_HPP
#define NV_FILE_UTILS_HPP
#include <fstream>
#include <string>
#include <vector>
#include "Graph.hpp"
#include "Node.hpp"

void write_adjacency_list(const Adjacency_List& G, const std::string& fname)
{
    std::ofstream f(fname);
    for (const Node_Connections& nodeCon: G)
    {
        for (const Connection& con: nodeCon.connections)
        {
            f << nodeCon.source_ID;
            f << "," << con.target_idx << "," << con.weight << "\n";
        }
    }
    f.close();
}

void write_node_properties(const std::vector<Node>& nodes, const std::string& fname)
{
    std::ofstream f(fname);
    for (const Node& node: nodes)
    {
        f << node.idx << "," << node.position[0] << "," << node.position[1] << '\n';
    }
}

#endif