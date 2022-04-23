#ifndef NV_MENU_HPP
#define NV_MENU_HPP
#include <map>
#include <unordered_set>
#include <vector>
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
void dispatchMenuWindows(const std::unordered_set<std::string>& activeMenus)
{
    for (auto& menu : activeMenus)
    {
        if (menu == "New")
        {

        }
        else if(menu == "Open")
        {
        }
        else if(menu == "Save")
        {
        }
        else if(menu == "New Network")
        {
            createGraphDesignerMenu();
        }
        else if(menu == "Import")
        {

        }
        else if(menu == "Preferences")
        {

        }
    }
}


void createTopMenu(UISettings& uiSettings, std::unordered_set<std::string>& activeMenus)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
       {
            const std::vector<std::string> menuNames = {"New", "Open", "Save", "New Network", "Import", "Preferences"};        
            for (const auto& menuName: menuNames)
            {
                if (ImGui::MenuItem(menuName.c_str()))
                {
                    activeMenus.insert(menuName.c_str());
                }
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