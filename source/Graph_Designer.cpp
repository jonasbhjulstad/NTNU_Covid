#include <algorithm>
#include <memory>
#include <limits>
#include <imgui/imgui.h>
#include <igraph/igraph_games.h>
#include <stdexcept>
#include <VulkanViewport/Graph_Designer.hpp>
namespace VkVP
{


GraphDesignStatus createGraphDesignerMenu(igraph_t* graph)
{
    if (graph == nullptr)
    {
        std::runtime_error("No provided pointer to graph creation object!");
    }
    GraphDesignStatus status = GRAPH_DESIGN_STATUS_IDLE;
    static GraphDesignPage current_page = GRAPH_DESIGN_GENERATION;
    static GraphGenerationParam genParam;
    static GraphLayoutParam layoutParam;
    switch (current_page)
    {
        case GRAPH_DESIGN_GENERATION:
        {
            status = displayGraphGeneration(graph, genParam);
            break;
        }
        case GRAPH_DESIGN_LAYOUT:
        {
            status = displayGraphLayout(graph, layoutParam);
            break;
        }
    }
    return status;
}

GraphDesignStatus displayGraphGeneration(igraph_t* graph, GraphGenerationParam& param)
{
    GraphDesignStatus status = GRAPH_DESIGN_STATUS_IDLE;
        // std::unique_ptr<igraph_t> p_graph = std::make_unique(igraph_empty(0, IGRAPH_UNDIRECTED));
    if (ImGui::Begin("Graph Generation", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
    {

        // General BeginCombo() API, you have full control over your selection data and display type.
        // (your selection data could be an index, a pointer to the object, an id for the object, a flag stored in the object itself, etc.)
        const char *graphTypes[] = {"Erdös-Rényi", "Barabási-Albert", "Watts-Strogatz"};
        if (ImGui::BeginCombo("Something", param.graphType, ImGuiComboFlags_NoArrowButton)) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < IM_ARRAYSIZE(graphTypes); n++)
            {
                bool is_selected = (param.graphType == graphTypes[n]);
                if (ImGui::Selectable(graphTypes[n], is_selected))
                    param.graphType = graphTypes[n];
                if (is_selected)
                    ImGui::SetItemDefaultFocus(); // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
            ImGui::EndCombo();
        }

        auto io = ImGui::GetIO();


        ImGuiInputTextFlags decInputFlags = ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AlwaysOverwrite;

        if (param.graphType == "Erdös-Rényi")
        {
            const char *ER_types[] = {"GNM", "GNP"};

            if (param.ERType == "GNP")
            {
                ImGui::InputInt("Nodes", &param.N_nodes, 1, GRAPH_CREATION_MAX_NODES, decInputFlags);
                ImGui::InputFloat("Probability", &param.p);//, 1.0f, 10.0f, -1,".3f", decInputFlags);
            
            }
            else if (param.ERType == "GNM")
            {
                ImGui::InputInt("Nodes", &param.N_nodes, 1, GRAPH_CREATION_MAX_NODES, decInputFlags);
                ImGui::InputInt("Edges", &param.N_edges, 0, GRAPH_CREATION_MAX_EDGES, decInputFlags);
            }
        }
        else if (param.graphType == "Barabási-Albert")
        {
            ImGui::InputInt("Nodes", &param.N_nodes, 1, GRAPH_CREATION_MAX_NODES, decInputFlags);
            ImGui::InputInt("Edges", &param.N_edges, 0, GRAPH_CREATION_MAX_EDGES, decInputFlags);
            ImGui::Text("p(d) ~ d^power + A");
            ImGui::InputFloat("Preferential power", &param.power);
            ImGui::InputFloat("Preferential constant A", &param.A);
            ImGui::InputInt("Outgoing edges per vertex", &param.m, 0, GRAPH_CREATION_MAX_EDGES, ImGuiInputTextFlags_CharsDecimal);
        }
        else if (param.graphType == "Watts-Strogatz")
        {

            ImGui::InputInt("Dimension", &param.dim, 1, 1000);
            ImGui::InputInt("Nodes", &param.size, 1, GRAPH_CREATION_MAX_NODES);
            ImGui::InputInt("Neighborhood Size", &param.neigborhoodSize, 1, 1000);
            ImGui::InputFloat("Rewiring Probability", &param.rewireProbability);
            ImGui::Checkbox("Loops", &param.loops);
            ImGui::Checkbox("Multiple Edges", &param.multipleEdges);
        }

        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            status = GRAPH_DESIGN_STATUS_CANCELED;
        }
        ImGui::SameLine();
        if (ImGui::Button("Next", ImVec2(120, 0)))
        {
            status = GRAPH_DESIGN_STATUS_NEXT;
        }

        ImGui::End();
    }
    return status;
}

GraphDesignStatus displayGraphLayout(igraph_t* graph, GraphLayoutParam& param)
{
    GraphDesignStatus status = GRAPH_DESIGN_STATUS_IDLE;
    if (ImGui::Begin("Graph Layout", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
    {
        const char *layoutTypes[] = {"Circle", "Random", "Fruchterman-Reingold", "Kamada-Kawai"};
        if (ImGui::BeginCombo("GraphLayout", param.layoutType, ImGuiComboFlags_NoArrowButton)) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < IM_ARRAYSIZE(layoutTypes); n++)
            {
                bool is_selected = (param.layoutType == layoutTypes[n]);
                if (ImGui::Selectable(layoutTypes[n], is_selected))
                    param.layoutType = layoutTypes[n];
                if (is_selected)
                    ImGui::SetItemDefaultFocus(); // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
            ImGui::EndCombo();
        }
        ImGui::End();
    }
    return status;
}

GraphDesignStatus createGraph(igraph_t* graph, const GraphGenerationParam& genParam, const GraphLayoutParam& layoutParam)
{
    GraphDesignStatus status = GRAPH_DESIGN_STATUS_IDLE;
    if (genParam.graphType == "Erdös-Rényi")
    {
        if (genParam.ERType == "GNP")
        {
            igraph_erdos_renyi_game_gnp(graph, genParam.N_nodes, genParam.p, IGRAPH_UNDIRECTED, 0);
        }
        else if (genParam.ERType == "GNM")
        {
            igraph_erdos_renyi_game_gnm(graph, genParam.N_nodes, genParam.N_edges, IGRAPH_UNDIRECTED, 0);
        }
    }
    else if (genParam.graphType == "Barabási-Albert")
    {
        igraph_barabasi_game(graph, genParam.N_nodes, genParam.power, genParam.N_edges, nullptr, 1, genParam.A, 0, IGRAPH_BARABASI_PSUMTREE, nullptr);
    }
    else if (genParam.graphType == "Watts-Strogatz")
    {
        igraph_watts_strogatz_game(graph, genParam.dim, genParam.size, genParam.neigborhoodSize, genParam.rewireProbability, genParam.loops, genParam.multipleEdges);
    }
    return GRAPH_DESIGN_STATUS_GRAPH_CREATED;
}

}