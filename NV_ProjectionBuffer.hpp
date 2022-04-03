#ifndef NV_PROJECTION_BUFFER_HPP
#define NV_PROJECTION_BUFFER_HPP
#include <chrono>
#include "vulkanexamplebase.h"
#include "VulkanglTFModel.h"

class ProjectionBuffer
{
	public:
	vks::Buffer buffer;

	struct UBO_Projection
	{
		glm::mat4 projection;
		glm::mat4 modelview;
		glm::vec4 lightPos;
		float theta;
	} uboProjection;

		// Prepare and initialize uniform buffer containing shader uniforms
	void prepare(vks::VulkanDevice* vulkanDevice);
	void update(Camera& camera, float frameTimer);

	~ProjectionBuffer()
	{
		buffer.destroy();
	}
};


#endif
