#ifndef MENU_HPP
#define MENU_HPP
#include <imgui/imgui.h>
#include <igraph/igraph.h>
#include "UISettings.hpp"
#include "Menu_Window_Defines.hpp"
namespace Menu
{
void createPreferencesMenu(ImVec4 *nodeStateColors);
void dispatchMenuWindows(std::map<Menu_Window, bool> &activeMenus, igraph_t* graph);
void createTopMenu(UISettings &uiSettings);
}
#endif