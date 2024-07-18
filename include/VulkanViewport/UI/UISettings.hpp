#ifndef UISETTINGS_HPP
#define UISETTINGS_HPP
#include <string>
#include <array>
#include <map>
#include <imgui/imgui.h>
#include <VulkanViewport/Editor/Menu_Window_Defines.hpp>
struct UISettings
{
	std::string fontPath;
	float fontSize = .5f;
};

#endif