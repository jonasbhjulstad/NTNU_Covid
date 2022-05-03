#ifndef NV_MENU_HPP
#define NV_MENU_HPP
#include <map>
#include <unordered_set>
#include <vector>
#include <imgui.h>
#include "NV_Graph_Designer.hpp"
#include "NV_UISettings.hpp"

void createPreferencesMenu(ImVec4 *nodeStateColors);
void dispatchMenuWindows(std::map<NV_Menu_Window, bool> &activeMenus);
void createTopMenu(UISettings &uiSettings);

void createPopupMenu(bool &p_active);

#endif