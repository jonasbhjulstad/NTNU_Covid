#ifndef NV_ERDOS_RENYI
#define NV_ERDOS_RENYI
#include "Graph.hpp"
#include "Math_Utils.hpp"
#include <cstdint>
#include <random>

Adjacency_List generate_Erdos_Renyi(const uint32_t N, const double p)
{
    Adjacency_List G;
    G.reserve((uint32_t)choose(N, 2) * p);
    uint32_t N_edges_max = N * (N - 1);

    std::random_device rnd;
    std::mt19937 gen(rnd());

    std::uniform_real_distribution<double> dist(0, 1);

    for (uint32_t node_i = 0; node_i < (N); node_i++)
    {
        G.push_back(Node_Connections{});
        G[node_i].source_ID = node_i;
        for (uint32_t node_j = 0; node_j < (N); node_j++)
        {
            if ((node_i != node_j) && dist(gen) < p)
            {
                G[node_i].connections.push_back(Connection{node_j, 1.0});
            }
        }
    }
    return G;
}

#endif