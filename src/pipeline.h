#pragma once

#include "graphics.h"

struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;
	glm::vec2 uv;

	enum Location {
		Position,
		Color,
		Uv,
		LocationCount
	};

	static auto getBindingInfo()
	{
		VkVertexInputBindingDescription inputBinding;
		inputBinding.binding = 0;
		inputBinding.stride = sizeof(Vertex);
		inputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return inputBinding;
	}

	static auto getAttribs()
	{
		auto attribs = std::vector<VkVertexInputAttributeDescription>(LocationCount);

		attribs[Position].location = 0;
		attribs[Position].binding = 0;
		attribs[Position].format = VK_FORMAT_R32G32B32_SFLOAT;
		attribs[Position].offset = offsetof(Vertex, pos);

		attribs[Color].location = 1;
		attribs[Color].binding = 0;
		attribs[Color].format = VK_FORMAT_R32G32B32_SFLOAT;
		attribs[Color].offset = offsetof(Vertex, color);

		attribs[Uv].location = 2;
		attribs[Uv].binding = 0;
		attribs[Uv].format = VK_FORMAT_R32G32_SFLOAT;
		attribs[Uv].offset = offsetof(Vertex, uv);

		return attribs;
	}
};

struct UBO {
	glm::mat4 mvp;
};

class Pipeline {
public:
	Pipeline(VkPhysicalDevice physicalDevice, VkDevice device, VkRenderPass renderPass, std::size_t imageCount);
	~Pipeline();

	VkDevice device;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformMemories;
	std::vector<VkDescriptorSetLayout> descriptorLayouts;
	VkShaderModule shaderVert;
	VkShaderModule shaderFrag;
	VkPipelineLayout layout;
	VkPipeline pipeline;
};
