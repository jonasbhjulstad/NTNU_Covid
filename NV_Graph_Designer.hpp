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

GraphDesignStatus createGraphDesignerMenu(igraph_t* graph)
{
    GraphDesignStatus status = GRAPH_DESIGN_STATUS_IDLE;
    // std::unique_ptr<igraph_t> p_graph = std::make_unique(igraph_empty(0, IGRAPH_UNDIRECTED));
    if (ImGui::BeginPopupModal("New Graph", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
    {

        // General BeginCombo() API, you have full control over your selection data and display type.
        // (your selection data could be an index, a pointer to the object, an id for the object, a flag stored in the object itself, etc.)
        const char *graphTypes[] = {"Erdös-Rényi", "Barabási-Albert", "Watts-Strogatz"};
        static const char *currentGraphType = graphTypes[0];                        // Here our selection is a single pointer stored outside the object.
        if (ImGui::BeginCombo("", currentGraphType, ImGuiComboFlags_NoArrowButton)) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < IM_ARRAYSIZE(graphTypes); n++)
            {
                bool is_selected = (currentGraphType == graphTypes[n]);
                if (ImGui::Selectable(graphTypes[n], is_selected))
                    currentGraphType = graphTypes[n];
                if (is_selected)
                    ImGui::SetItemDefaultFocus(); // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
            ImGui::EndCombo();
        }

        auto io = ImGui::GetIO();

        static int N_nodes = 1;

        const ImGuiInputTextFlags decInputFlags = ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AlwaysInsertMode;

        const char *ER_types[] = {"GNM", "GNP"};
        static const char *currentERType = ER_types[0];
        static float p = .5;
        static int N_edges = 0;
        static float power = 1.f;
        static float A = 1.f;
        static int m = 1;
        static int dim = 2;
        static int size = 5;
        static int neigborhoodSize = 2;
        static float rewireProbability = .0;
        static bool loops = 0;
        static bool multipleEdges = 0;
        if (currentGraphType == "Erdös-Rényi")
        {
            // if (ImGui::BeginCombo("", currentERType, ImGuiComboFlags_NoArrowButton))
            // {
            //     for (int n = 0; n < IM_ARRAYSIZE(ER_types); n++)
            //     {
            //         bool is_selected = (currentERType == ER_types[n]);
            //         if (ImGui::Selectable(ER_types[n], is_selected))
            //             currentERType = ER_types[n];
            //         if (is_selected)
            //             ImGui::SetItemDefaultFocus(); // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            //     }
            //     ImGui::EndCombo();
            // }

            if (currentERType == "GNP")
            {
                ImGui::InputInt("Nodes", &N_nodes, 1, GRAPH_CREATION_MAX_NODES, decInputFlags);
                ImGui::InputFloat("Probability", &p, .0f, 1.f, decInputFlags);
            }
            else if (currentERType == "GNM")
            {
                ImGui::InputInt("Nodes", &N_nodes, 1, GRAPH_CREATION_MAX_NODES, decInputFlags);
                ImGui::InputInt("Edges", &N_edges, 0, GRAPH_CREATION_MAX_EDGES, decInputFlags);
            }
        }
        else if (currentGraphType == "Barabási-Albert")
        {
            ImGui::InputInt("Nodes", &N_nodes, 1, GRAPH_CREATION_MAX_NODES, decInputFlags);
            ImGui::InputInt("Edges", &N_edges, 0, GRAPH_CREATION_MAX_EDGES, decInputFlags);
            ImGui::Text("p(d) ~ d^power + A");
            ImGui::InputFloat("Preferential power", &power, .0f, 100.f, decInputFlags);
            ImGui::InputFloat("Preferential constant A", &A, .0f, std::numeric_limits<float>::infinity(), ImGuiInputTextFlags_CharsDecimal);
            ImGui::InputInt("Outgoing edges per vertex", &m, 0, GRAPH_CREATION_MAX_EDGES, ImGuiInputTextFlags_CharsDecimal);
        }
        else if (currentGraphType == "Watts-Strogatz")
        {

            ImGui::InputInt("Dimension", &dim, 1, 1000);
            ImGui::InputInt("Nodes", &size, 1, GRAPH_CREATION_MAX_NODES);
            ImGui::InputInt("Neighborhood Size", &neigborhoodSize, 1, 1000);
            ImGui::InputFloat("Rewiring Probability", &rewireProbability, .0f, 1.f);
            ImGui::Checkbox("Loops", &loops);
            ImGui::Checkbox("Multiple Edges", &multipleEdges);
        }

        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            status = GRAPH_DESIGN_STATUS_CANCELED;
        }
        ImGui::SameLine();
        if (ImGui::Button("Create", ImVec2(120, 0)))
        {
            if (currentGraphType == "Erdös-Rényi")
            {
                if (currentERType == "GNP")
                {
                    igraph_erdos_renyi_game_gnp(graph, N_nodes, p, IGRAPH_UNDIRECTED, 0);
                }
                else if (currentERType == "GNM")
                {
                    igraph_erdos_renyi_game_gnm(graph, N_nodes, N_edges, IGRAPH_UNDIRECTED, 0);
                }
            }
            else if (currentGraphType == "Barabási-Albert")
            {
                igraph_barabasi_game(graph, N_nodes, power, N_edges, nullptr, 1, A, 0, IGRAPH_BARABASI_PSUMTREE, nullptr);
            }
            else if (currentGraphType == "Watts-Strogatz")
            {
                igraph_watts_strogatz_game(graph, dim, size, neigborhoodSize, rewireProbability, loops, multipleEdges);
            }
            status = GRAPH_DESIGN_STATUS_GRAPH_CREATED;
        }

        ImGui::End();
    }
    return status;
}

#endif