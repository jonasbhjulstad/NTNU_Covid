#ifndef POPUP_MENU_HPP
#define POPUP_MENU_HPP
#include <imgui.h>

void createPopupMenu(bool &p_active)
{
    if (p_active)
    {
        if (ImGui::IsMouseClicked(1))
        {
            ImGui::SetNextWindowPos(ImGui::GetMousePos());
        }

        static int selected_fish = -1;
        const char *names[] = {"Bream", "Haddock", "Mackerel", "Pollock", "Tilefish"};
        static bool toggles[] = {true, false, false, false, false};
        if (ImGui::BeginPopup("select"))
        {
            ImGui::Text("Aquarium");
            ImGui::Separator();
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                if (ImGui::Selectable(names[i]))
                    selected_fish = i;
            ImGui::EndPopup();
        }
    }
}

#endif