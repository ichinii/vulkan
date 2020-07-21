#pragma once

#include "instance.h"
#include "pipeline.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>

namespace PolygonPipeline {

struct Ubo {
	glm::mat4 mvp = glm::mat4(1.f);
};

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
};

using Vertices = std::vector<Vertex>;

inline auto createPipeline(const Instance& instance) {
	auto textures = Textures();
	textures.emplace_back(Texture::fromGenerator(instance, glm::uvec2(8, 8), [] (glm::vec2 uv) {
		uv = glm::abs(uv);
		return glm::vec4(uv.y, 0, uv.x, 1);
	}));
	textures.emplace_back(Texture::fromGenerator(instance, glm::uvec2(64, 64), [] (glm::vec2 uv) {
			return glm::vec4(0, glm::sin(uv.x * 6.f + uv.y * 4.f), 0, 1);
	}));


	auto uniforms = Uniforms();
	uniforms.push_back(createUniform(0, VK_SHADER_STAGE_VERTEX_BIT, UniformBuffer::fromStruct<Ubo>(instance)));
	uniforms.push_back(createUniform(1, VK_SHADER_STAGE_FRAGMENT_BIT, std::move(textures[0])));
	uniforms.push_back(createUniform(2, VK_SHADER_STAGE_FRAGMENT_BIT, std::move(textures[1])));

	auto attributes = Attributes {
		Attribute {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(PolygonPipeline::Vertex, pos)},
		Attribute {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(PolygonPipeline::Vertex, color)},
		Attribute {2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(PolygonPipeline::Vertex, uv)},
	};

	// auto vertexShader = VertexShader("res/shader.vert.spv", attributes, uniforms);

	auto pipeline = std::make_unique<Pipeline>(instance, std::move(uniforms), attributes, sizeof(PolygonPipeline::Vertex));
	return std::make_tuple(std::move(pipeline), getUniformBuffers(pipeline->uniforms), getUniformTextures(pipeline->uniforms));
}

}
