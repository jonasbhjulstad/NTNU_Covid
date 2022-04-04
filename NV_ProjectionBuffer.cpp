#include "NV_ProjectionBuffer.hpp"

		// Prepare and initialize uniform buffer containing shader uniforms
	void ProjectionBuffer::prepare(vks::VulkanDevice* vulkanDevice)
	{
		// Vertex shader uniform buffer block
		VK_CHECK_RESULT(vulkanDevice->createBuffer(
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&buffer,
			sizeof(UBO_Projection),
			&uboProjection));
	}


	void ProjectionBuffer::update(Camera& camera, float frameTimer, UISettings& uiSettings)
	{
		// Vertex shader
		uboProjection.projection = camera.matrices.perspective;
		uboProjection.modelview = camera.matrices.view * glm::mat4(1.0f);

		// Light source
		if (uiSettings.animateLight)
		{
			uiSettings.lightTimer += frameTimer * uiSettings.lightSpeed;
			uboProjection.lightPos.x = sin(glm::radians(uiSettings.lightTimer * 360.0f)) * 15.0f;
			uboProjection.lightPos.z = cos(glm::radians(uiSettings.lightTimer * 360.0f)) * 15.0f;
		};


		VK_CHECK_RESULT(buffer.map());
		memcpy(buffer.mapped, &uboProjection, sizeof(UBO_Projection));
		buffer.unmap();
	}