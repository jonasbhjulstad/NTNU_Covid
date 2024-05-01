#ifndef GRAPH_LAYOUT_HPP
#define GRAPH_LAYOUT_HPP
#include <vector>
#include <igraph/igraph.h>
#include <igraph/igraph_layout.h>
#include <VulkanTools/NodeInstance.hpp>
#include <VulkanTools/EdgeInstance.hpp>

namespace VkVP
{
    std::vector<NodeInstanceData> kamada_kawai_2D(const igraph_t& graph, size_t max_iter, float epsilon)
    {
        size_t N_nodes = igraph_vcount(&graph);
        igraph_matrix_t pos;
        igraph_matrix_init(&pos, N_nodes, 2);
        //get number of nodes in graph
        //fill igraph_vector with n 1s
        igraph_vector_t weights;
        size_t N_edges = igraph_ecount(&graph);
        igraph_vector_init(&weights, N_edges);
        igraph_vector_fill(&weights, 1);
        igraph_layout_circle(&graph, &pos, igraph_vss_all());
        igraph_matrix_scale(&pos, 100);
        double kkconst = 1;
        igraph_layout_kamada_kawai(&graph, &pos, true, max_iter, epsilon, kkconst, &weights, NULL, NULL, NULL, NULL);

        std::vector<NodeInstanceData> node_data;
        for (size_t i = 0; i < N_nodes;i++)
        {
            node_data.push_back({{MATRIX(pos,i,0), MATRIX(pos,i,1), 0}, {1.f,1.f,1.f, .8f},1.f});
        }
        return node_data;
    } 

    std::vector<NodeInstanceData> kamada_kawai_3D(const igraph_t& graph, size_t max_iter, float epsilon)
    {
        size_t N_nodes = igraph_vcount(&graph);
        igraph_matrix_t pos;
        igraph_matrix_init(&pos, N_nodes, 3);
        //get number of nodes in graph
        //fill igraph_vector with n 1s
        igraph_vector_t weights;
        igraph_vector_init(&weights, N_nodes);
        igraph_vector_fill(&weights, 1.);

        double kkconst = N_nodes;
        igraph_layout_kamada_kawai(&graph, &pos, false, max_iter, epsilon, kkconst, &weights, NULL, NULL, NULL, NULL);

        std::vector<NodeInstanceData> node_data;
        for (size_t i = 0; i < N_nodes;i++)
        {
            node_data.push_back({{MATRIX(pos,i,0), MATRIX(pos,i,1), MATRIX(pos,i,2)}, {1.f,1.f,1.f, .8f},1.f});
        }
        return node_data;
    }

    std::vector<EdgeInstanceData> get_edge_positions(const std::vector<NodeInstanceData>& nodeInstanceData, const igraph_t& graph)
    {
        std::vector<EdgeInstanceData> edge_data;
        //write edge node positions to edge_data
        for (size_t i = 0; i < igraph_ecount(&graph); i++)
        {
            //get edge
            igraph_integer_t from, to;
            igraph_edge(&graph, i, &from, &to);
            //get node positions
            glm::vec3 startNode = nodeInstanceData[from].pos;
            glm::vec3 endNode = nodeInstanceData[to].pos;
            glm::vec3 scale = {1.f,1.f,1.f};
            edge_data.push_back({startNode, endNode, scale});
        }
        return edge_data;
    }   
}
#endif