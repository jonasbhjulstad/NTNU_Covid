#ifndef NV_GRAPH_DESIGNER_HPP
#define NV_GRAPH_DESIGNER_HPP
#include <memory>
#include <igraph/igraph.h>
#include "imgui.h"


std::unique_ptr<igraph_t> newGraphMenu()
{
    std::unique_ptr<igraph_t> p_graph = std::make_unique(igraph_empty(0, IGRAPH_UNDIRECTED));
    if (ImGui::Begin("New Graph", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::Button("Erdos Renyi"))
        {
            ImGui::InputInt("Nodes", &nodes);
        }
        ImGui::SameLine();
        if (ImGui::Button("Barabasi-Albert"))
        {
            
        }
        ImGui::End();
    }
}



#endif