#pragma once

#include "uniformbuffer.h"
#include "texture.h"
#include "util.h"
#include <variant>

struct UniformInfo {
	int binding;
	VkDescriptorType type;
	VkShaderStageFlagBits shaderStage;

	bool operator== (const UniformInfo& other) const { return binding == other.binding && type == other.type && shaderStage == other.shaderStage; }
};
using UniformInfos = std::vector<UniformInfo>;

struct Uniform : UniformInfo {
	// Uniform() = default;
	// Uniform(const Uniform&) = delete;
	// Uniform(Uniform&&) = default;
	//
	// Uniform& operator= (const Uniform&) = delete;
	// Uniform& operator= (Uniform&&) = default;

	using Buffer = std::variant<UniformBuffer, Texture>;
	Buffer buffer;
};
using Uniforms = std::vector<Uniform>;

inline Uniform createUniform(int binding, VkShaderStageFlagBits stage, UniformBuffer buffer)
{
	auto type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	auto info = UniformInfo {binding, type, stage};
	return Uniform {info, std::move(buffer)};
}

inline Uniform createUniform(int binding, VkShaderStageFlagBits stage, Texture texture)
{
	auto type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	auto info = UniformInfo {binding, type, stage};
	return Uniform {info, std::move(texture)};
}

// std::vector<UniformBuffer*> getUniformBuffers(Uniforms& uniforms)
// {
// 	std::vector<UniformBuffer*> buffers;
// 	for (auto& uniform : uniforms) {
// 		std::visit(overloaded{
// 			[&] (UniformBuffer& buffer) {
// 				buffers.push_back(&buffer);
// 			}, [] (auto&) {}
// 		}, uniform.buffer);
// 	}
// 	return buffers;
// }
//
// std::vector<Texture*> getUniformTextures(Uniforms& uniforms)
// {
// 	std::vector<Texture*> textures;
// 	for (auto& uniform : uniforms) {
// 		std::visit(overloaded{
// 			[&] (Texture& texture) {
// 				textures.push_back(&texture);
// 			}, [] (auto&) {}
// 		}, uniform.buffer);
// 	}
// 	return textures;
// }
