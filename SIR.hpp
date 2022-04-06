/*
 * Vulkan Example - imGui (https://github.com/ocornut/imgui)
 *
 * Copyright (C) 2017 by Sascha Willems - www.saschawillems.de
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "vulkanexamplebase.h"
#include "VulkanglTFModel.h"
#include <random>
#include <chrono>
#define ENABLE_VALIDATION true
#include "computeBasic.hpp"




// ----------------------------------------------------------------------------
// VulkanExample
// ----------------------------------------------------------------------------

#ifdef WIN32
	const std::string assetPath = "C:\\Users\\jonas\\Documents\\Vulkan\\examples\\imgui\\data\\";
	const std::string shadersPath = assetPath + "shaders\\";
    const std::string computeShadersPath = assetPath + "computeShaders";
	const std::string modelPath = assetPath + "models\\";
	const std::string texturePath = assetPath + "textures\\";
#else
	const std::string assetPath = "/home/deb/Documents/Vulkan/examples/imgui/data/";
	const std::string shadersPath = assetPath + "shaders/";
    const std::string computeShadersPath = assetPath + "computeShaders";
	const std::string modelPath = assetPath + "models/";
	const std::string texturePath = assetPath + "textures/";
#endif

class VulkanExample: public VulkanExampleBase
{
	void render() {};
};

using namespace BasicCompute;
int main()
{
	auto vulkanExample = new VulkanExample();															
	vulkanExample->initVulkan();																	
	vulkanExample->setupWindow();					 												
	vulkanExample->prepare();	

}