#ifndef NETWORK_VIEWPORT_SHADER_MODULE_HPP
#define NETWORK_VIEWPORT_SHADER_MODULE_HPP
#include <vector>
#include <vulkan/vulkan.hpp>
#include <fstream>
static std::vector<char> readShader(const std::string &shaderPath);
static VkShaderModule createShaderModule(const std::vector<char> &code);

#endif
