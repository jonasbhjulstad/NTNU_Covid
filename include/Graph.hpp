#ifndef NV_GRAPH_HPP
#define NV_GRAPH_HPP
#include <vector>
#include "Edge.hpp"
#include "Node.hpp"



struct Connection
{
    uint32_t target_idx;
    double weight;
};

struct Node_Connections
{
    uint32_t source_ID;
    std::vector<Connection> connections;
    const uint32_t degree()
    {
        return connections.size();
    }
};

typedef std::vector<Node_Connections> Adjacency_List;


inline uint32_t degree(const Adjacency_List& G, const uint32_t idx)
{
    return G[idx].connections.size();
}

inline Adjacency_List compute_k_core(const Adjacency_List& G, const uint32_t degree)
{
    Adjacency_List res;
    std::for_each(G.begin(), G.end(), [&res](const auto& connections)
    {if (connections.degree() < k)
    res.push_back(connections)});
}

inline std::vector<Adjacency_List> compute_k_cores(const Adjacency_List& G, const uint32_t degree_max)
{
    std::vector<Adjacency_List> k_cores(degree_max);
    Adjacency_List k_core_list = G;
    for (uint32_t i = degree_max; i > 0; i--)
    {
        k_cores.push_back(compute_k_core(k_cores.end(), i));
    }
}


#endif