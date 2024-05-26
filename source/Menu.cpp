#include <VulkanViewport/Menu.hpp>
#include <map>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include <VulkanViewport/Graph_Designer.hpp>

namespace VkVP
{

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
void dispatchMenuWindows(std::map<Menu_Window, bool> &activeMenus)
{
    for (auto p_menu = activeMenus.begin(); p_menu != activeMenus.end();)
    {
        bool erase_entry = false;
        if (p_menu->second)
        {
            if (p_menu->first == MENU_WINDOW_NEW)
            {
            }
            else if (p_menu->first == MENU_WINDOW_OPEN)
            {
            }
            else if (p_menu->first == MENU_WINDOW_SAVE)
            {
            }
            else if ((p_menu->first == MENU_WINDOW_NEW_GRAPH))
            {
            }
            else if (p_menu->first == MENU_WINDOW_IMPORT)
            {
            }
            else if (p_menu->first == MENU_WINDOW_PREFERENCES)
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
            for (const auto &menu : Menu_Window_Names)
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

}