#ifndef NV_VULKAN_INITIALIZERS_HPP
#define NV_VULKAN_INITIALIZERS_HPP

#include <vector>
#include <vulkan/vulkan.hpp>

namespace initializers
{
	 VkMemoryAllocateInfo memoryAllocateInfo();

	 VkMappedMemoryRange mappedMemoryRange();

		 VkCommandBufferAllocateInfo commandBufferAllocateInfo(
		VkCommandPool commandPool,
		VkCommandBufferLevel level,
		uint32_t bufferCount);
	 VkCommandPoolCreateInfo commandPoolCreateInfo();

	 VkCommandBufferBeginInfo commandBufferBeginInfo();

	 VkCommandBufferInheritanceInfo commandBufferInheritanceInfo();

	 VkRenderPassBeginInfo renderPassBeginInfo();

	 VkRenderPassCreateInfo renderPassCreateInfo();

	 VkImageMemoryBarrier imageMemoryBarrier();

	 VkBufferMemoryBarrier bufferMemoryBarrier();

	 VkMemoryBarrier memoryBarrier();

	 VkImageCreateInfo imageCreateInfo();

	 VkSamplerCreateInfo samplerCreateInfo();

	 VkImageViewCreateInfo imageViewCreateInfo();

	 VkFramebufferCreateInfo framebufferCreateInfo();

	 VkSemaphoreCreateInfo semaphoreCreateInfo();

	 VkFenceCreateInfo fenceCreateInfo(VkFenceCreateFlags flags);

	 VkEventCreateInfo eventCreateInfo();

	 VkSubmitInfo submitInfo();

	 VkViewport viewport(
		float width,
		float height,
		float minDepth,
		float maxDepth);

			 VkRect2D rect2D(
		int32_t width,
		int32_t height,
		int32_t offsetX,
		int32_t offsetY);
	 VkBufferCreateInfo bufferCreateInfo();

	 VkBufferCreateInfo bufferCreateInfo(
		VkBufferUsageFlags usage,
		VkDeviceSize size);
	 VkDescriptorPoolCreateInfo descriptorPoolCreateInfo(
		uint32_t poolSizeCount,
		VkDescriptorPoolSize *pPoolSizes,
		uint32_t maxSets);
	 VkDescriptorPoolCreateInfo descriptorPoolCreateInfo(
		const std::vector<VkDescriptorPoolSize> &poolSizes,
		uint32_t maxSets);
	 VkDescriptorPoolSize descriptorPoolSize(
		VkDescriptorType type,
		uint32_t descriptorCount);
	 VkDescriptorSetLayoutBinding descriptorSetLayoutBinding(
		VkDescriptorType type,
		VkShaderStageFlags stageFlags,
		uint32_t binding,
		uint32_t descriptorCount);
	 VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo(
		const VkDescriptorSetLayoutBinding *pBindings,
		uint32_t bindingCount);


	 VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo(
		const std::vector<VkDescriptorSetLayoutBinding> &bindings);

	 VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo(
		const VkDescriptorSetLayout *pSetLayouts,
		uint32_t setLayoutCount);

	 VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo(
		uint32_t setLayoutCount);
	 VkDescriptorSetAllocateInfo descriptorSetAllocateInfo(
		VkDescriptorPool descriptorPool,
		const VkDescriptorSetLayout *pSetLayouts,
		uint32_t descriptorSetCount);
	 VkDescriptorImageInfo descriptorImageInfo(VkSampler sampler, VkImageView imageView, VkImageLayout imageLayout);


	 VkWriteDescriptorSet writeDescriptorSet(
		VkDescriptorSet dstSet,
		VkDescriptorType type,
		uint32_t binding,
		VkDescriptorBufferInfo *bufferInfo,
		uint32_t descriptorCount);

	 VkWriteDescriptorSet writeDescriptorSet(
		VkDescriptorSet dstSet,
		VkDescriptorType type,
		uint32_t binding,
		VkDescriptorImageInfo *imageInfo,
		uint32_t descriptorCount);
	 VkVertexInputBindingDescription vertexInputBindingDescription(
		uint32_t binding,
		uint32_t stride,
		VkVertexInputRate inputRate);

	 VkVertexInputAttributeDescription vertexInputAttributeDescription(
		uint32_t binding,
		uint32_t location,
		VkFormat format,
		uint32_t offset);
	 VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo();

	 VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo(
		const std::vector<VkVertexInputBindingDescription> &vertexBindingDescriptions,
		const std::vector<VkVertexInputAttributeDescription> &vertexAttributeDescriptions);
	 VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(
		VkPrimitiveTopology topology,
		VkPipelineInputAssemblyStateCreateFlags flags,
		VkBool32 primitiveRestartEnable);

	 VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo(
		VkPolygonMode polygonMode,
		VkCullModeFlags cullMode,
		VkFrontFace frontFace,
		VkPipelineRasterizationStateCreateFlags flags);
	 VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState(
		VkColorComponentFlags colorWriteMask,
		VkBool32 blendEnable);
	 VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(
		uint32_t attachmentCount,
		const VkPipelineColorBlendAttachmentState *pAttachments);
	 VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(
		VkBool32 depthTestEnable,
		VkBool32 depthWriteEnable,
		VkCompareOp depthCompareOp);

	 VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo(
		uint32_t viewportCount,
		uint32_t scissorCount,
		VkPipelineViewportStateCreateFlags flags);
	 VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo(
		VkSampleCountFlagBits rasterizationSamples,
		VkPipelineMultisampleStateCreateFlags flags);

	 VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(
		const VkDynamicState *pDynamicStates,
		uint32_t dynamicStateCount,
		VkPipelineDynamicStateCreateFlags flags);
	 VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(
		const std::vector<VkDynamicState> &pDynamicStates,
		VkPipelineDynamicStateCreateFlags flags);
	 VkPipelineTessellationStateCreateInfo pipelineTessellationStateCreateInfo(uint32_t patchControlPoints);

	 VkGraphicsPipelineCreateInfo pipelineCreateInfo(
		VkPipelineLayout layout,
		VkRenderPass renderPass,
		VkPipelineCreateFlags flags);

	 VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo(
		const std::vector<VkDescriptorSetLayoutBinding> &bindings);

	 VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo(
		const VkDescriptorSetLayout *pSetLayouts,
		uint32_t setLayoutCount);

	 VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo(
		uint32_t setLayoutCount);
	 VkDescriptorSetAllocateInfo descriptorSetAllocateInfo(
		VkDescriptorPool descriptorPool,
		const VkDescriptorSetLayout *pSetLayouts,
		uint32_t descriptorSetCount);
	 VkDescriptorImageInfo descriptorImageInfo(VkSampler sampler, VkImageView imageView, VkImageLayout imageLayout);


	 VkWriteDescriptorSet writeDescriptorSet(
		VkDescriptorSet dstSet,
		VkDescriptorType type,
		uint32_t binding,
		VkDescriptorBufferInfo *bufferInfo,
		uint32_t descriptorCount);

	 VkWriteDescriptorSet writeDescriptorSet(
		VkDescriptorSet dstSet,
		VkDescriptorType type,
		uint32_t binding,
		VkDescriptorImageInfo *imageInfo,
		uint32_t descriptorCount);
	 VkVertexInputBindingDescription vertexInputBindingDescription(
		uint32_t binding,
		uint32_t stride,
		VkVertexInputRate inputRate);

	 VkVertexInputAttributeDescription vertexInputAttributeDescription(
		uint32_t binding,
		uint32_t location,
		VkFormat format,
		uint32_t offset);
	 VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo();

	 VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo(
		const std::vector<VkVertexInputBindingDescription> &vertexBindingDescriptions,
		const std::vector<VkVertexInputAttributeDescription> &vertexAttributeDescriptions);
	 VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(
		VkPrimitiveTopology topology,
		VkPipelineInputAssemblyStateCreateFlags flags,
		VkBool32 primitiveRestartEnable);

	 VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo(
		VkPolygonMode polygonMode,
		VkCullModeFlags cullMode,
		VkFrontFace frontFace,
		VkPipelineRasterizationStateCreateFlags flags);
	 VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState(
		VkColorComponentFlags colorWriteMask,
		VkBool32 blendEnable);
	 VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(
		uint32_t attachmentCount,
		const VkPipelineColorBlendAttachmentState *pAttachments);
	 VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(
		VkBool32 depthTestEnable,
		VkBool32 depthWriteEnable,
		VkCompareOp depthCompareOp);

	 VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo(
		uint32_t viewportCount,
		uint32_t scissorCount,
		VkPipelineViewportStateCreateFlags flags);
	 VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo(
		VkSampleCountFlagBits rasterizationSamples,
		VkPipelineMultisampleStateCreateFlags flags);

	 VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(
		const VkDynamicState *pDynamicStates,
		uint32_t dynamicStateCount,
		VkPipelineDynamicStateCreateFlags flags);
	 VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(
		const std::vector<VkDynamicState> &pDynamicStates,
		VkPipelineDynamicStateCreateFlags flags);
	 VkPipelineTessellationStateCreateInfo pipelineTessellationStateCreateInfo(uint32_t patchControlPoints);

	 VkGraphicsPipelineCreateInfo pipelineCreateInfo(
		VkPipelineLayout layout,
		VkRenderPass renderPass,
		VkPipelineCreateFlags flags);
	 VkGraphicsPipelineCreateInfo pipelineCreateInfo();


	 VkComputePipelineCreateInfo computePipelineCreateInfo(
		VkPipelineLayout layout,
		VkPipelineCreateFlags flags);

}
#endif