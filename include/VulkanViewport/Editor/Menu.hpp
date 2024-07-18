#ifndef MENU_HPP
#define MENU_HPP
#include <imgui/imgui.h>
#include <VulkanViewport/UI/UISettings.hpp>
#include <VulkanViewport/Editor/Menu_Window_Defines.hpp>
namespace VkVP
{
void createTopMenu(UISettings &uiSettings);
void createMenus();
}
#endif