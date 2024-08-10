#pragma once
#include <glm/glm.hpp>
struct LightingData {
  glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  glm::vec4 ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
};