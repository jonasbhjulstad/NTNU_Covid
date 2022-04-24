#ifndef NV_UTILS_HPP
#define NV_UTILS_HPP
#include <imgui.h>

ImVec4 rgbNormalize(const ImVec4& color)
{
    return {color.x/255.0, color.y/255.0, color.z/255.0, color.w};
}

#endif