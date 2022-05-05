#include "NV_ProjectionBuffer.hpp"
	void prepareProjectionBuffer(VulkanDevice* vulkanDevice, VulkanBuffer& buffer, ProjectionData& projectionData, const Camera& camera)
	{
		VK_CHECK_RESULT(vulkanDevice->createBuffer(
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&buffer,
			sizeof(ProjectionData)));

		// Map persistent
		VK_CHECK_RESULT(buffer.map());

		updateProjectionBuffer(buffer, projectionData, camera, true);
	}

	void updateProjectionBuffer(VulkanBuffer& buffer, ProjectionData& projectionData, const Camera& camera, bool viewChanged)
	{
		if (viewChanged)
		{
			projectionData.projection = camera.matrices.perspective;
			projectionData.view = camera.matrices.view;
		}

		memcpy(buffer.mapped, &projectionData, sizeof(ProjectionData));
	}