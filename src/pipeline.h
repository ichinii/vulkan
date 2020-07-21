#pragma once

#include "graphics.h"
#include "instance.h"
#include "uniformbuffer.h"
#include "texture.h"
#include "resource.h"
#include <vector>
#include <variant>

// uniforms

struct UniformInfo {
	int binding;
	VkDescriptorType type;
	VkShaderStageFlagBits shaderStage;
};

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

template <typename ...Ts>
struct overloaded : public Ts... { using Ts::operator()...; };
template <typename ...Ts>
overloaded(Ts...) -> overloaded<Ts...>;

extern Uniform createUniform(int binding, VkShaderStageFlagBits stage, UniformBuffer buffer);
extern Uniform createUniform(int binding, VkShaderStageFlagBits stage, Texture image);
extern std::vector<UniformBuffer*> getUniformBuffers(Uniforms& uniforms);
extern std::vector<Texture*> getUniformTextures(Uniforms& uniforms);

// attributes

struct Attribute {
	int location;
	int binding;
	VkFormat format;
	std::uint32_t offset;
};

using Attributes = std::vector<Attribute>;

// pipeline

class Pipeline {
public:
	Pipeline(const Instance& instance, Uniforms&& uniforms, Attributes attributes, std::size_t vertexSize);
	Pipeline(const Pipeline&) = delete;
	Pipeline(Pipeline&&) = default;
	~Pipeline();

	Pipeline& operator= (const Pipeline&) = delete;
	Pipeline& operator= (Pipeline&&) = default;

	template <typename ...Ts>
	void updateUniforms(Ts&&... uniforms);

	VkDevice device;
	std::vector<VkDescriptorSetLayout> descriptorLayouts;
	Uniforms uniforms;
	Attributes attributes;
	Resource<VkShaderModule> shaderVert = VK_NULL_HANDLE;
	Resource<VkShaderModule> shaderFrag = VK_NULL_HANDLE;
	Resource<VkPipelineLayout> layout = VK_NULL_HANDLE;
	Resource<VkPipeline> pipeline = VK_NULL_HANDLE;
	Resource<VkDescriptorPool> descriptorPool = VK_NULL_HANDLE;
  Resource<std::vector<VkDescriptorSet>> descriptorSets;
};
