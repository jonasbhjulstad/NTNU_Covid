#ifndef NV_MENU_HPP
#define NV_MENU_HPP
#include "imgui.h"
#include "NV_UISettings.hpp"
#include "NV_Graph_Designer.hpp"
void createPreferencesMenu(ImVec4* nodeStateColors)
{
    if(ImGui::Begin("Preferences"))
    {
        ImGui::ColorEdit4("Suceptible", (float*)&nodeStateColors[0], ImGuiColorEditFlags_Float);
        ImGui::ColorEdit4("Infected", (float*)&nodeStateColors[1], ImGuiColorEditFlags_Float);
        ImGui::ColorEdit4("Recovered", (float*)&nodeStateColors[2], ImGuiColorEditFlags_Float);
        ImGui::End();
    }
}

void createTopMenu(UISettings& uiSettings)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {

            ImGui::MenuItem("New");
            ImGui::MenuItem("Open");
            ImGui::MenuItem("Save");
            if (ImGui::MenuItem("New Network"))
            {
                createGraphDesignerMenu();
            }
            ImGui::MenuItem("Import");
            if(ImGui::MenuItem("Preferences"))
            {
                uiSettings.prefMenu = true;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::MenuItem("Undo");
            ImGui::MenuItem("Redo");
            ImGui::MenuItem("Cut");
            ImGui::MenuItem("Copy");
            ImGui::MenuItem("Paste");
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

}



#endif