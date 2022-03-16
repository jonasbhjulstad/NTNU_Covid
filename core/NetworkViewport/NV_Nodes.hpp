#ifndef NV_NODES_HPP
#define NV_NODES_HPP
#include <glm/glm.hpp>

struct Node
{
    float radius = 10.f;
    float borderWidth = 1.f;
    glm::vec2 position;
    glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    glm::vec4 borderColor = {.0f, .0f, .0f, 1.0f};
};

#endif