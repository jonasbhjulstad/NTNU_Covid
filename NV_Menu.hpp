#ifndef NV_MENU_HPP
#define NV_MENU_HPP
#include <map>
#include <unordered_set>
#include <vector>
#include <imgui.h>
#include "NV_UISettings.hpp"
#include "NV_Graph_Designer.hpp"

enum NV_Menu_Window
{
    NV_MENU_WINDOW_NEW,
    NV_MENU_WINDOW_OPEN,
    NV_MENU_WINDOW_SAVE,
    NV_MENU_WINDOW_NEW_GRAPH,
    NV_MENU_WINDOW_IMPORT,
    NV_MENU_WINDOW_PREFERENCES
};

const std::map<NV_Menu_Window, std::string> NV_Menu_Window_Names = {
    {NV_MENU_WINDOW_NEW, "New"},
    {NV_MENU_WINDOW_OPEN, "Open"},
    {NV_MENU_WINDOW_SAVE, "Save"},
    {NV_MENU_WINDOW_NEW_GRAPH, "New Graph"},
    {NV_MENU_WINDOW_IMPORT, "Import"},
    {NV_MENU_WINDOW_PREFERENCES, "Preferences"}
};

void createPreferencesMenu(ImVec4 *nodeStateColors)
{
    if (ImGui::Begin("Preferences"))
    {
        ImGui::ColorEdit4("Suceptible", (float *)&nodeStateColors[0], ImGuiColorEditFlags_Float);
        ImGui::ColorEdit4("Infected", (float *)&nodeStateColors[1], ImGuiColorEditFlags_Float);
        ImGui::ColorEdit4("Recovered", (float *)&nodeStateColors[2], ImGuiColorEditFlags_Float);
        ImGui::End();
    }
}
void dispatchMenuWindows(std::map<NV_Menu_Window, bool> &activeMenus)
{
    for (auto p_menu = activeMenus.begin(); p_menu != activeMenus.end();)
    {
        bool erase_entry = false;
        if (p_menu->second)
        {
            if (p_menu->first == NV_MENU_WINDOW_NEW)
            {
            }
            else if (p_menu->first == NV_MENU_WINDOW_OPEN)
            {
            }
            else if (p_menu->first == NV_MENU_WINDOW_SAVE)
            {
            }
            else if (p_menu->first == NV_MENU_WINDOW_NEW_GRAPH)
            {
                igraph_t graph;
                switch(createGraphDesignerMenu(&graph))
                {
                    case GRAPH_DESIGN_STATUS_CANCELED:
                        erase_entry = true;
                        break;
                    case GRAPH_DESIGN_STATUS_GRAPH_CREATED:
                        igraph_destroy(&graph);
                        erase_entry = true;
                        break;
                }
            }
            else if (p_menu->first == NV_MENU_WINDOW_IMPORT)
            {
            }
            else if (p_menu->first == NV_MENU_WINDOW_PREFERENCES)
            {
            }

            if (erase_entry)
            {
                p_menu = activeMenus.erase(p_menu);
            }
            else
            {
                p_menu++;
            }
        }
    }
}

void createTopMenu(UISettings &uiSettings, std::map<NV_Menu_Window, bool> &activeMenus)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            for (const auto &menu : NV_Menu_Window_Names)
            {
                if (ImGui::MenuItem(menu.second.c_str()))
                {
                    activeMenus[menu.first] = true;
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