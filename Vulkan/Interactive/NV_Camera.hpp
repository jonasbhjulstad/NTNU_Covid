/*
* Basic camera class
*
* Copyright (C) 2016 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef NV_CAMERA_HPP
#define NV_CAMERA_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <algorithm>
#include <imgui.h>
#include <imgui_internal.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera
{
private:
	float fov;
	float znear, zfar;
	ImGuiWindow* window;
	ImGuiContext* imguiContext;
	void updateViewMatrix();
public:
	ImVec2 mousePos_old = {.0f,.0f};

	enum CameraType { lookat, firstperson };
	CameraType type = CameraType::lookat;

	glm::vec3 rotation = glm::vec3();
	glm::vec3 position = glm::vec3();
	glm::vec4 viewPos = glm::vec4();

	float rotationSpeed = 1.0f;
	float movementSpeed = 1.0f;

	bool updated = false;
	bool flipY = false;

	struct
	{
		glm::mat4 perspective;
		glm::mat4 view;
	} matrices;

	void setContext(ImGuiContext* _context){ window = _context->NavWindow; imguiContext = _context;}

	bool moving()
	{
		ImGuiIO& io = ImGui::GetIO();
		return io.KeysDown[ImGuiKey_W] || io.KeysDown[ImGuiKey_A] || io.KeysDown[ImGuiKey_D] || io.KeysDown[ImGuiKey_S];
	}

	float getNearClip() { 
		return znear;
	}

	float getFarClip() {
		return zfar;
	}

	void setPerspective(float fov, float aspect, float znear, float zfar);

	void updateAspectRatio(float aspect);

	void setPosition(glm::vec3 position);

	void setRotation(glm::vec3 rotation);

	void rotate(glm::vec3 delta);

	void setTranslation(glm::vec3 translation);

	void translate(glm::vec3 delta);

	void setRotationSpeed(float rotationSpeed);

	void setMovementSpeed(float movementSpeed);

	void update(float deltaTime);
	void mouseUpdate(ImVec2 pos);

};
#endif