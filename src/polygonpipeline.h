#pragma once

#include "instance.h"
#include "pipeline.h"
#include "texture.h"
#include "uniform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>
#include <ranges>

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

inline UniformInfo toUniformInfo(Uniform& uniform) {
	return uniform;
}

inline auto getUniformInfos() {
	return UniformInfos {
		UniformInfo {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
		UniformInfo {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
		UniformInfo {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
	};
}

inline auto getAttributeInfos() {
	return AttributeInfos {
		AttributeInfo {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(PolygonPipeline::Vertex, pos)},
		AttributeInfo {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(PolygonPipeline::Vertex, color)},
		AttributeInfo {2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(PolygonPipeline::Vertex, uv)},
	};
}

inline auto createUniforms(const Instance& instance) {
	auto textures = Textures();
	textures.emplace_back(Texture::fromGenerator(instance, glm::uvec2(8, 8), [] (glm::vec2 uv) {
		uv = glm::abs(uv);
		return glm::vec4(uv.y, 0, uv.x, 1);
	}));
	textures.emplace_back(Texture::fromGenerator(instance, glm::uvec2(64, 64), [] (glm::vec2 uv) {
			return glm::vec4(0, glm::sin(uv.x * 6.f + uv.y * 4.f), 0, 1);
	}));

	auto uniforms = Uniforms();
	uniforms.push_back(createUniform(0, VK_SHADER_STAGE_VERTEX_BIT, UniformBuffer::fromStruct<PolygonPipeline::Ubo>(instance)));
	uniforms.push_back(createUniform(1, VK_SHADER_STAGE_FRAGMENT_BIT, std::move(textures[0])));
	uniforms.push_back(createUniform(2, VK_SHADER_STAGE_FRAGMENT_BIT, std::move(textures[1])));

	return uniforms;
}

inline auto createPipeline(const Instance& instance, VkRenderPass renderPass) {
	auto textures = Textures();
	textures.emplace_back(Texture::fromGenerator(instance, glm::uvec2(8, 8), [] (glm::vec2 uv) {
		uv = glm::abs(uv);
		return glm::vec4(uv.y, 0, uv.x, 1);
	}));
	textures.emplace_back(Texture::fromGenerator(instance, glm::uvec2(64, 64), [] (glm::vec2 uv) {
			return glm::vec4(0, glm::sin(uv.x * 6.f + uv.y * 4.f), 0, 1);
	}));

	auto pipeline = std::make_unique<Pipeline>(
		instance,
		renderPass,
		getUniformInfos(),
		getAttributeInfos(),
		sizeof(PolygonPipeline::Vertex)
	);

	return pipeline;
}

}
