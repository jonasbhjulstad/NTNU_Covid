#ifndef NV_MATH_UTILS_HPP
#define NV_MATH_UTILS_HPP
#include <cstdint>
uint32_t choose(const uint32_t, const uint32_t);

uint32_t choose(const uint32_t n, const uint32_t k)
{
    if (k == 0) 
        return 1;
    return (n * choose(n - 1, k - 1)) / k;
}
#endif