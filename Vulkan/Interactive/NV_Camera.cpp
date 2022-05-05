#include "NV_Camera.hpp"

	void Camera::updateViewMatrix()
	{
		glm::mat4 rotM = glm::mat4(1.0f);
		glm::mat4 transM;

		rotM = glm::rotate(rotM, glm::radians(rotation.x * (flipY ? -1.0f : 1.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
		rotM = glm::rotate(rotM, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		rotM = glm::rotate(rotM, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 translation = position;
		if (flipY) {
			translation.y *= -1.0f;
		}
		transM = glm::translate(glm::mat4(1.0f), translation);

		if (type == CameraType::firstperson)
		{
			matrices.view = rotM * transM;
		}
		else
		{
			matrices.view = transM * rotM;
		}

		viewPos = glm::vec4(position, 0.0f) * glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);

		updated = true;
	};

	void Camera::setPerspective(float fov, float aspect, float znear, float zfar)
	{
		this->fov = fov;
		this->znear = znear;
		this->zfar = zfar;
		matrices.perspective = glm::perspective(glm::radians(fov), aspect, znear, zfar);
		if (flipY) {
			matrices.perspective[1][1] *= -1.0f;
		}
	};

	void Camera::updateAspectRatio(float aspect)
	{
		matrices.perspective = glm::perspective(glm::radians(fov), aspect, znear, zfar);
		if (flipY) {
			matrices.perspective[1][1] *= -1.0f;
		}
	}

	void Camera::setPosition(glm::vec3 position)
	{
		this->position = position;
		updateViewMatrix();
	}

	void Camera::setRotation(glm::vec3 rotation)
	{
		this->rotation = rotation;
		updateViewMatrix();
	}

	void Camera::rotate(glm::vec3 delta)
	{
		this->rotation += delta;
		updateViewMatrix();
	}

	void Camera::setTranslation(glm::vec3 translation)
	{
		this->position = translation;
		updateViewMatrix();
	};

	void Camera::translate(glm::vec3 delta)
	{
		this->position += delta;
		updateViewMatrix();
	}

	void Camera::setRotationSpeed(float rotationSpeed)
	{
		this->rotationSpeed = rotationSpeed;
	}

	void Camera::setMovementSpeed(float movementSpeed)
	{
		this->movementSpeed = movementSpeed;
	}

	void Camera::mouseUpdate(ImVec2 mousePos)
{
	ImVec2 delta = {mousePos_old[0] - mousePos[0], mousePos_old[1] - mousePos[1]};
	
	delta.x = (delta.x > 100.f) ? 100.f : delta.x;
	delta.x = (delta.x < -100.f) ? -100.f : delta.x;
	delta.y = (delta.y > 100.f) ? 100.f : delta.y;
	delta.y = (delta.y < -100.f) ? -100.f : delta.y;

	ImGuiIO& io = ImGui::GetIO();

	if (io.MouseDown[0]) {
		this->rotate(glm::vec3(delta[1] * rotationSpeed, -delta[0] * rotationSpeed, 0.0f));
	}
	if (io.MouseDown[1]) {
		this->translate(glm::vec3(-0.0f, 0.0f, delta[1] * .005f));
	}
	if (io.MouseDown[2]) {
		this->translate(glm::vec3(-delta[0] * 0.01f, -delta[1] * 0.01f, 0.0f));
	}
	mousePos_old = mousePos;
}

	void Camera::update(float deltaTime)
	{
		updated = false;
		ImGuiIO& io = ImGui::GetIO();
		ImGuiContext* imguiContext = ImGui::GetCurrentContext();

		if (imguiContext->NavWindow == this->window)
		{
		this->mouseUpdate(io.MousePos);

		if (type == CameraType::firstperson)
		{
			if (moving())
			{
				glm::vec3 camFront;
				camFront.x = -cos(glm::radians(rotation.x)) * sin(glm::radians(rotation.y));
				camFront.y = sin(glm::radians(rotation.x));
				camFront.z = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
				camFront = glm::normalize(camFront);

				float moveSpeed = deltaTime * movementSpeed;

				if (io.KeysDown[ImGuiKey_W])
					position += camFront * moveSpeed;
				if (io.KeysDown[ImGuiKey_S])
					position -= camFront * moveSpeed;
				if (io.KeysDown[ImGuiKey_A])
					position -= glm::normalize(glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f))) * moveSpeed;
				if (io.KeysDown[ImGuiKey_D])
					position += glm::normalize(glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f))) * moveSpeed;

				updateViewMatrix();
			}
		}
		}
	};


