#include <VulkanTools/Camera.hpp>
#include <VulkanViewport/Editor/Menu.hpp>
#include <map>
#include <stdexcept>
#include <unordered_set>
#include <vector>

namespace VkVP {

void showPerformanceStats(const std::array<float, 50> &frameTimes) {
  ImGui::PlotLines("Frame Times", frameTimes.data(), 50, 0, "", 0.0f, 120.0f,
                   ImVec2(0, 80));
}

void showCameraSettings(Camera &camera) {
  ImGui::Begin("Camera Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::InputFloat3("Position", &camera.position.x);
  ImGui::InputFloat3("Rotation", &camera.rotation.x);
  ImGui::SliderFloat("Movement speed", &camera.movementSpeed, 1.0f, 20.0f);
}

void showFontSettings() {
  static float fontSize = 1.0f;
  ImGui::SliderFloat("Font size", &fontSize, .1f, 10.0f);
  ImGui::GetIO().FontGlobalScale = fontSize;
}
void dispatchMenu(const Menu_Windows activeMenu) {
  switch (activeMenu) {
  case Menu_Windows::TOP_MENU_NEW:
    break;
  case Menu_Windows::TOP_MENU_OPEN:
    break;
  case Menu_Windows::TOP_MENU_SAVE:
    break;
  case Menu_Windows::TOP_MENU_IMPORT:
    break;
  case Menu_Windows::TOP_MENU_PREFERENCES:
    break;
  default:
    throw std::runtime_error("Invalid menu option");
    break;
  }
}

void createTopMenu(std::unordered_set<Menu_Windows> &activeMenus) {
  if (ImGui::BeginMainMenuBar()) {
    for (const auto &menu : Top_Menu_Names) {
      if (ImGui::BeginMenu(menu.second.c_str())) 
      {
        activeMenus.insert(menu.first);
        ImGui::EndMenu();
      }
    }
  }
  ImGui::EndMainMenuBar();
}

void createMenus() {
  static std::unordered_set<Menu_Windows> activeMenus{};
  createTopMenu(activeMenus);

  std::for_each(activeMenus.begin(), activeMenus.end(), dispatchMenu);
}

} // namespace VkVP