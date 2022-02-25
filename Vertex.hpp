#ifndef VERTEX_HPP
#define VERTEX_HPP
#include "imgui.h"
#include <utility>
#include <math.h>
#include <vector>
typedef std::pair<ImVec2, ImVec2> BoundingBox;

struct Graph_Vertex
{
    typedef boost::convex_topology<2>::point tPoint;
    unsigned int ID;
    unsigned int InputID;
    unsigned int OutputID;
    tPoint Position;
};
struct Graph_Edge
{
    unsigned int ID;
    double weight = 1.;
};

void DrawCircleVertices(const std::vector<ImVec2>& Positions,
const float& Radius,
const ImVec4& ColorFill,
const ImVec4& ColorEdge,
const ImVec2& offset = ImVec2{.0,.0},
const int& N_segments = 20,
const float& EdgeThickness = .0)
{
    ImU32 cFill = ImGui::GetColorU32(ColorFill);
    ImU32 cEdge = ImGui::GetColorU32(ColorEdge);
    ImDrawList* drawlist = ImGui::GetWindowDrawList();
    for (ImVec2 RelPos: Positions)
    {
        ImVec2 Pos = ImVec2{RelPos.x + offset.x, RelPos.y + offset.y};
        drawlist->AddCircleFilled(Pos, Radius, cFill, N_segments);
        drawlist->AddCircle(Pos, Radius, cEdge, N_segments, EdgeThickness? EdgeThickness : Radius/5.f);
    }
}
#endif