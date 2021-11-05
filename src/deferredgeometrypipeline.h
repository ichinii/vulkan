#pragma once

#include "graphics.h"
#include "pipeline.h"
#include "log.h"

namespace DeferredGeometryPipeline {

using namespace GraphicsPipeline;

struct Ubo {
	glm::mat4 mvp = glm::mat4(1.f);
};

struct Vertex {
	glm::vec3 pos;
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

inline auto getUniformInfos() {
	return UniformInfos {
		UniformInfo {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
		UniformInfo {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
		UniformInfo {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
	};
}

inline auto getAttributeInfos() {
	return AttributeInfos {
		AttributeInfo {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos)},
		AttributeInfo {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)},
		AttributeInfo {2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)},
	};
}

inline auto createDeferredGeometryPipeline(VkDevice device, VkRenderPass renderPass, std::size_t subpassIndex, VkPipelineLayout pipelineLayout, VkShaderModule shaderVert, VkShaderModule shaderFrag, const AttributeInfos& attributeInfos)
{
	auto shaderStageInfos = std::vector {
		getShaderStageVertInfo(shaderVert),
		getShaderStageFragInfo(shaderFrag),
	};
	auto vertexBindingInfo = getBindingInfo(sizeof(Vertex));
	auto vertexAttribs = getAttributeBindings(attributeInfos);
	auto vertexInputInfo = getVertexInputInfo(&vertexBindingInfo, vertexAttribs);
	auto inputAssemblyInfo = getInputAssemblyInfoTriangleList();
	auto viewport = getViewportFullscreen();
	auto scissor = getScissorFullscreen();
	auto viewportStateInfo = getViewportStateInfo(viewport, scissor);
	auto rasterizationInfo = getRasterizationInfo();
	auto multisampleInfo = getMultisampleInfo();
	auto depthStencilInfo = getDepthStencilInfoDepthEnabled();
	auto colorBlendAttachments = std::vector {
		getColorBlendAttachmentDisabled(),
		getColorBlendAttachmentDisabled(),
	};
	auto colorBlendInfo = getColorBlendInfo(colorBlendAttachments);
	auto dynamicStates = std::vector<VkDynamicState> {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
	};
	auto dynamicStateInfo = getDynamicStateInfo(dynamicStates);
	auto graphicsPipelineInfo = getGraphicsPipelineInfo(
		shaderStageInfos,
		vertexInputInfo,
		inputAssemblyInfo,
		viewportStateInfo,
		rasterizationInfo,
		multisampleInfo,
		depthStencilInfo,
		colorBlendInfo,
		dynamicStateInfo,
		pipelineLayout,
		renderPass,
		subpassIndex
	);

	VkPipeline graphicsPipeline;
	error << vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &graphicsPipelineInfo, nullptr, &graphicsPipeline);

	return graphicsPipeline;
}

inline auto createDeferredGeometryPipelineRaii(VkDevice device, VkShaderModule shaderVert, VkShaderModule shaderFrag, VkRenderPass renderPass, std::size_t subpassIndex) {
	auto uniformInfos = getUniformInfos();
	auto attributeInfos = getAttributeInfos();
	auto descriptorLayout = createGeometryDescriptorSetLayout(device, uniformInfos);
	auto layout = createPipelineLayout(device, descriptorLayout);
	auto pipeline = createDeferredGeometryPipeline(device, renderPass, subpassIndex, layout, shaderVert, shaderFrag, attributeInfos);
	auto descriptorPool = createGeometryDescriptorPool(device, uniformInfos);
	auto descriptorSet = createDescriptorSets(device, descriptorLayout, descriptorPool);
	return GraphicsPipelineRaii {
		device,
		layout,
		pipeline,
		descriptorLayout,
		descriptorPool,
		descriptorSet,
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
	uniforms.push_back(createUniform(0, VK_SHADER_STAGE_VERTEX_BIT, UniformBuffer::fromStruct<Ubo>(instance)));
	uniforms.push_back(createUniform(1, VK_SHADER_STAGE_FRAGMENT_BIT, std::move(textures[0])));
	uniforms.push_back(createUniform(2, VK_SHADER_STAGE_FRAGMENT_BIT, std::move(textures[1])));

	return uniforms;
}

} // namespace DeferredGeometryPipeline
