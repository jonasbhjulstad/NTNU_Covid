#include "NV_Menu.hpp"
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

void createTopMenu(UISettings &uiSettings)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            for (const auto &menu : NV_Menu_Window_Names)
            {
                if (ImGui::MenuItem(menu.second.c_str()))
                {
                    uiSettings.activeMenus[menu.first] = true;
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

void createPopupMenu(bool &p_active)
{
    if (p_active)
    {
        if (ImGui::IsMouseClicked(1))
        {
            ImGui::SetNextWindowPos(ImGui::GetMousePos());
        }

        static int selected_fish = -1;
        const char *names[] = {"Bream", "Haddock", "Mackerel", "Pollock", "Tilefish"};
        static bool toggles[] = {true, false, false, false, false};
        if (ImGui::BeginPopup("select"))
        {
            ImGui::Text("Aquarium");
            ImGui::Separator();
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                if (ImGui::Selectable(names[i]))
                    selected_fish = i;
            ImGui::EndPopup();
        }
    }
}
