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

template <typename ...Ts>
struct overloaded : public Ts... { using Ts::operator()...; };
template <typename ...Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// attributes

struct AttributeInfo {
	int location;
	int binding;
	VkFormat format;
	std::uint32_t offset;
};

using AttributeInfos = std::vector<AttributeInfo>;

// pipeline

class Pipeline {
public:
	Pipeline(const Instance& instance, UniformInfos uniformInfos, AttributeInfos attributeInfos, std::size_t vertexSize);
	Pipeline(const Pipeline&) = delete;
	Pipeline(Pipeline&&) = default;
	~Pipeline();

	Pipeline& operator= (const Pipeline&) = delete;
	Pipeline& operator= (Pipeline&&) = default;

	void updateUniforms(const Uniforms& uniforms);

	VkDevice device;
	UniformInfos uniformInfos;
	AttributeInfos attributeInfos;
	Resource<VkShaderModule> shaderVert = VK_NULL_HANDLE;
	Resource<VkShaderModule> shaderFrag = VK_NULL_HANDLE;
	Resource<VkPipelineLayout> layout = VK_NULL_HANDLE;
	Resource<VkPipeline> pipeline = VK_NULL_HANDLE;
	Resource<VkDescriptorPool> descriptorPool = VK_NULL_HANDLE;
	Resource<VkDescriptorSetLayout> descriptorLayout;
	Resource<VkDescriptorSet> descriptorSet;
};
