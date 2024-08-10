#include "menu.hpp"


void showLightSettings(LightingData& data)
{
    if(ImGui::Begin("Light Settings"))
    {
        ImGui::ColorEdit4("Light Color", (float*)&data.lightColor);
        ImGui::ColorEdit4("Ambient", (float*)&data.ambient);
        ImGui::End();
    }
}
