#ifndef NV_NODE_HPP
#define NV_NODE_HPP
#include <cstdint>
#include <array>
#include <casadi/casadi.hpp>

struct Node
{
    std::array<double, 2> position;
    uint32_t idx;
};

#endif