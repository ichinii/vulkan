#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>

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

class PlanarRenderer {
public:
	PlanarRenderer();
	~PlanarRenderer();

	void clear();
	std::vector<Vertex> flush();

	void setTexture();
	void setTransform(glm::mat4 modelMatrix);
	void drawTriangle(
		glm::vec2 p0, glm::vec3 c0, glm::vec2 t0,
		glm::vec2 p1, glm::vec3 c1, glm::vec2 t1,
		glm::vec2 p2, glm::vec3 c2, glm::vec2 t2);
	void drawCircle(glm::vec2 p, float r, glm::vec3 c);

private:
	std::vector<Vertex> m_vertices;
};
