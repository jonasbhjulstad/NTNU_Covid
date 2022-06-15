#ifndef NV_MENU_HPP
#define NV_MENU_HPP
#include <imgui.h>
#include <igraph/igraph.h>
#include "NV_UISettings.hpp"
#include "NV_Menu_Window_Defines.hpp"
namespace Menu
{
void createPreferencesMenu(ImVec4 *nodeStateColors);
void dispatchMenuWindows(std::map<NV_Menu_Window, bool> &activeMenus, igraph_t* graph);
void createTopMenu(UISettings &uiSettings);
}
#endif