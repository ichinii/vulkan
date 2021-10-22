#pragma once

#include "pipeline.h"

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
