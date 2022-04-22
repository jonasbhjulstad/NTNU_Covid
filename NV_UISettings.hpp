#ifndef NV_UISETTINGS_HPP
#define NV_UISETTINGS_HPP
#include <string>
#include <array>
#include "imgui.h"
struct UISettings
{
	bool displayNodes = true;
	bool displayEdges = true;
	bool animateLight = false;
	float lightSpeed = 0.25f;
	std::array<float, 50> frameTimes{};
	float frameTimeMin = 9999.0f, frameTimeMax = 0.0f;
	float lightTimer = 0.0f;
	bool popup = false;
	float fontPixels = 64.0;
	float fontSize = .5*(fontPixels)/64.0;
	std::string fontPath;
	ImVec4 nodeStateColors[3] = {ImVec4{1.0,1.0,.0,1.0}, ImVec4{1.0,.0,.0,1.0}, ImVec4{.0,.0,1.0,1.0}};
	bool prefMenu = false;
};

#endif