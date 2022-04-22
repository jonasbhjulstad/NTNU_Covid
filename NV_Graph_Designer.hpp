#ifndef NV_GRAPH_DESIGNER_HPP
#define NV_GRAPH_DESIGNER_HPP
#include "imgui.h"


void createGraphDesignerMenu()
{
    if (ImGui::Begin("Graph designer"))
    {
        ImGui::Text("Graph designer");
        ImGui::End();
    }
}



#endif