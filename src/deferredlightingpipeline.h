#pragma once

#include "graphics.h"
#include "pipeline.h"
#include "log.h"

namespace DeferredLightingPipeline {

using namespace GraphicsPipeline;

struct Ubo {
	glm::mat4 p = glm::mat4(1.f);
	glm::vec2 windowSize;
};

inline auto getUniformInfos() {
	return UniformInfos {
		UniformInfo {2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT},
	};
}

inline auto createDeferredLightingPipeline(VkDevice device, VkRenderPass renderPass, std::size_t subpassIndex, VkPipelineLayout pipelineLayout, VkShaderModule shaderVert, VkShaderModule shaderFrag)
{
	auto shaderStageInfos = std::vector {
		getShaderStageVertInfo(shaderVert),
		getShaderStageFragInfo(shaderFrag),
	};
	auto vertexAttribs = getAttributeBindings({});
	auto vertexInputInfo = getVertexInputInfo(nullptr, vertexAttribs);
	auto inputAssemblyInfo = getInputAssemblyInfoTriangleStrip();
	auto viewport = getViewportFullscreen();
	auto scissor = getScissorFullscreen();
	auto viewportStateInfo = getViewportStateInfo(viewport, scissor);
	auto rasterizationInfo = getRasterizationInfo();
	auto multisampleInfo = getMultisampleInfo();
	auto depthStencilInfo = getDepthStencilInfoDisabled();
	auto colorBlendAttachments = std::vector {
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

inline auto createDeferredLightingPipelineRaii(VkDevice device, VkShaderModule shaderVert, VkShaderModule shaderFrag, VkRenderPass renderPass, std::size_t subpassIndex) {
	auto uniformInfos = getUniformInfos();
	auto descriptorSetLayout = createLightingDescriptorSetLayout(device, uniformInfos);
	auto layout = createPipelineLayout(device, descriptorSetLayout);
	auto pipeline = createDeferredLightingPipeline(device, renderPass, subpassIndex, layout, shaderVert, shaderFrag);
	auto descriptorPool = createLightingDescriptorPool(device, uniformInfos);
	auto descriptorSet = createDescriptorSets(device, descriptorSetLayout, descriptorPool);
	return GraphicsPipelineRaii {
		device,
		layout,
		pipeline,
		descriptorSetLayout,
		descriptorPool,
		descriptorSet,
	};
}

inline auto createUniforms(const Instance& instance) {
	auto uniforms = Uniforms();
	uniforms.push_back(createUniform(2, VK_SHADER_STAGE_FRAGMENT_BIT, UniformBuffer::fromStruct<Ubo>(instance)));

	return uniforms;
}

} // namespace DeferredLightingPipeline
