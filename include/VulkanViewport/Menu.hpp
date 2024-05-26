#ifndef MENU_HPP
#define MENU_HPP
#include <imgui/imgui.h>
#include <igraph/igraph.h>
#include <VulkanViewport/UISettings.hpp>
#include <VulkanViewport/Menu_Window_Defines.hpp>
namespace VkVP
{
void createPreferencesMenu(ImVec4 *nodeStateColors);
void dispatchMenuWindows(std::map<Menu_Window, bool> &activeMenus);
void createTopMenu(UISettings &uiSettings);
}
#endif