#pragma once

#include <vector>
#include <variant>
#include "instance.h"
#include "uniform.h"
#include "attribute.h"
#include "resource.h"
#include "log.h"

namespace GraphicsPipeline {

inline auto getUniformBindings(const UniformInfos& uniformInfos) {
	auto bindings = std::vector<VkDescriptorSetLayoutBinding>(uniformInfos.size());

	for (std::size_t i = 0; i < uniformInfos.size(); ++i) {
		auto& uniformInfo = uniformInfos[i];
		auto& binding = bindings[i];
		binding.binding = uniformInfo.binding;
		binding.descriptorType = uniformInfo.type;
		binding.descriptorCount = 1;
		binding.stageFlags = uniformInfo.shaderStage;
		binding.pImmutableSamplers = nullptr; // Optional
	}

	return bindings;
}

inline auto createDescriptorSetLayout(VkDevice device, const UniformInfos& uniformInfos)
{
	auto bindings = getUniformBindings(uniformInfos);

	VkDescriptorSetLayoutCreateInfo layoutInfo;
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.pNext = nullptr;
	layoutInfo.flags = 0;
	layoutInfo.bindingCount = bindings.size();
	layoutInfo.pBindings = bindings.data();

	VkDescriptorSetLayout descriptorSetLayout;
	error << vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout);

	return descriptorSetLayout;
}

inline auto createLightingDescriptorSetLayout(VkDevice device)
{
	VkDescriptorSetLayoutBinding albedoInputAttachmentBinding;
	albedoInputAttachmentBinding.binding = 0;
	albedoInputAttachmentBinding.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	albedoInputAttachmentBinding.descriptorCount = 1;
	albedoInputAttachmentBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	albedoInputAttachmentBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutBinding depthInputAttachmentBinding;
	depthInputAttachmentBinding.binding = 1;
	depthInputAttachmentBinding.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	depthInputAttachmentBinding.descriptorCount = 1;
	depthInputAttachmentBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	depthInputAttachmentBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutBinding bindings[2] {
		albedoInputAttachmentBinding,
		depthInputAttachmentBinding,
	};

	VkDescriptorSetLayoutCreateInfo layoutInfo;
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.pNext = nullptr;
	layoutInfo.flags = 0;
	layoutInfo.bindingCount = 2;
	layoutInfo.pBindings = bindings;

	VkDescriptorSetLayout descriptorSetLayout;
	error << vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout);

	return descriptorSetLayout;
}

inline auto createPipelineLayout(VkDevice device, const VkDescriptorSetLayout& descriptorLayout)
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pNext = nullptr;
	pipelineLayoutInfo.flags = 0;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	VkPipelineLayout pipelineLayout;
	error << vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout);
	return pipelineLayout;
}

inline auto createDescriptorPool(VkDevice device, const UniformInfos& uniformInfos, std::size_t size)
{
	auto poolSizes = std::vector<VkDescriptorPoolSize>(uniformInfos.size());
	for (std::size_t i = 0; i < uniformInfos.size(); ++i) {
		auto& uniform = uniformInfos[i];
		poolSizes[i] = {uniform.type, static_cast<uint32_t>(size)};
	}

	VkDescriptorPoolCreateInfo poolInfo;
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = 0;
	poolInfo.maxSets = size;
	poolInfo.pNext = nullptr;
	poolInfo.poolSizeCount = poolSizes.size();
	poolInfo.pPoolSizes = poolSizes.data();

	VkDescriptorPool pool;
	error << vkCreateDescriptorPool(device, &poolInfo, nullptr, &pool);

	return pool;
}

inline auto createLightingDescriptorPool(VkDevice device, std::size_t size)
{
	VkDescriptorPoolSize albedoPoolSize;
	albedoPoolSize.descriptorCount = 1;
	albedoPoolSize.type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

	VkDescriptorPoolSize depthPoolSize;
	depthPoolSize.descriptorCount = 1;
	depthPoolSize.type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

	VkDescriptorPoolSize poolSizes[2] {
		albedoPoolSize,
		depthPoolSize,
	};

	VkDescriptorPoolCreateInfo poolInfo;
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = 0;
	poolInfo.maxSets = size;
	poolInfo.pNext = nullptr;
	poolInfo.poolSizeCount = 2;
	poolInfo.pPoolSizes = poolSizes;

	VkDescriptorPool pool;
	error << vkCreateDescriptorPool(device, &poolInfo, nullptr, &pool);

	return pool;
}

inline auto createDescriptorSets(VkDevice device, VkDescriptorSetLayout layout, VkDescriptorPool pool)
{
	VkDescriptorSetAllocateInfo allocInfo;
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &layout;

	auto set = VkDescriptorSet();
	error << vkAllocateDescriptorSets(device, &allocInfo, &set);

	return set;
}

inline auto updateDescriptor(
		VkDevice device,
		VkDescriptorSet& descriptorSet,
		const Uniforms& uniforms)
{
	auto bufferInfos = std::vector<VkDescriptorBufferInfo>(uniforms.size());
	auto imageInfos = std::vector<VkDescriptorImageInfo>(uniforms.size());

	for (std::size_t j = 0; j < uniforms.size(); ++j) {
		auto& uniform = uniforms[j];

		std::visit(overloaded{
			[&] (const UniformBuffer& buffer) {
				auto& bufferInfo = bufferInfos[j];
				bufferInfo.buffer = buffer.buffer;
				bufferInfo.offset = 0;
				bufferInfo.range = buffer.size;
			},
			[&] (const Texture& texture) {
				auto& imageInfo = imageInfos[j];
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo.imageView = texture.view;
				imageInfo.sampler = texture.sampler;
			},
		}, uniform.buffer);
	}

	auto descriptorWrites = std::vector<VkWriteDescriptorSet>(uniforms.size());
	for (std::size_t j = 0; j < uniforms.size(); ++j) {
		auto& uniform = uniforms[j];
		auto& write = descriptorWrites[j];
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.pNext = nullptr;
		write.dstSet = descriptorSet;
		write.dstBinding = uniform.binding;
		write.dstArrayElement = 0;
		write.descriptorCount = 1;
		write.descriptorType = uniform.type;
		write.pBufferInfo = &bufferInfos[j];
		write.pImageInfo = &imageInfos[j];
		write.pTexelBufferView = nullptr;
	}

	vkUpdateDescriptorSets(device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
}

inline auto updateDescriptor(
		VkDevice device,
		VkDescriptorSet& descriptorSet,
		VkImageView albedoView,
		VkImageView depthView)
{
	VkDescriptorImageInfo albedoImageInfo;
	albedoImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	albedoImageInfo.imageView = albedoView;
	albedoImageInfo.sampler = VK_NULL_HANDLE;

	VkDescriptorImageInfo depthImageInfo;
	depthImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	depthImageInfo.imageView = depthView;
	depthImageInfo.sampler = VK_NULL_HANDLE;

	VkWriteDescriptorSet albedoWrite;
	albedoWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	albedoWrite.pNext = nullptr;
	albedoWrite.dstSet = descriptorSet;
	albedoWrite.dstBinding = 0;
	albedoWrite.dstArrayElement = 0;
	albedoWrite.descriptorCount = 1;
	albedoWrite.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	albedoWrite.pBufferInfo = nullptr;
	albedoWrite.pImageInfo = &albedoImageInfo;
	albedoWrite.pTexelBufferView = nullptr;

	VkWriteDescriptorSet depthWrite;
	depthWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	depthWrite.pNext = nullptr;
	depthWrite.dstSet = descriptorSet;
	depthWrite.dstBinding = 1;
	depthWrite.dstArrayElement = 0;
	depthWrite.descriptorCount = 1;
	depthWrite.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	depthWrite.pBufferInfo = nullptr;
	depthWrite.pImageInfo = &depthImageInfo;
	depthWrite.pTexelBufferView = nullptr;
	
	VkWriteDescriptorSet writes[2] {
		albedoWrite,
		depthWrite,
	};

	vkUpdateDescriptorSets(device, 2, writes, 0, nullptr);
}

inline auto getShaderStageVertInfo(const VkShaderModule shaderVert) {
	VkPipelineShaderStageCreateInfo shaderStageVertInfo;
	shaderStageVertInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageVertInfo.pNext = nullptr;
	shaderStageVertInfo.flags = 0;
	shaderStageVertInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStageVertInfo.module = shaderVert;
	shaderStageVertInfo.pName = "main";
	shaderStageVertInfo.pSpecializationInfo = nullptr;
	return shaderStageVertInfo;
}

inline auto getShaderStageFragInfo(const VkShaderModule shaderFrag) {
	VkPipelineShaderStageCreateInfo shaderStageFragInfo;
	shaderStageFragInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageFragInfo.pNext = nullptr;
	shaderStageFragInfo.flags = 0;
	shaderStageFragInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStageFragInfo.module = shaderFrag;
	shaderStageFragInfo.pName = "main";
	shaderStageFragInfo.pSpecializationInfo = nullptr;
	return shaderStageFragInfo;
}

inline auto getBindingInfo(int vertexSize)
{
	VkVertexInputBindingDescription inputBinding;
	inputBinding.binding = 0;
	inputBinding.stride = vertexSize;
	inputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return inputBinding;
}

inline auto getAttributeBindings(const AttributeInfos& attributeInfos)
{
	auto bindings = std::vector<VkVertexInputAttributeDescription>(attributeInfos.size());

	for (std::size_t i = 0; i < attributeInfos.size(); ++i) {
		auto& binding = bindings[i];
		auto& attribute = attributeInfos[i];

		binding.location = attribute.location;
		binding.binding = attribute.binding;
		binding.format = attribute.format;
		binding.offset = attribute.offset;
	}

	return bindings;
}

inline auto getVertexInputInfo(
		const VkVertexInputBindingDescription* vertexBindingInfo,
		const std::vector<VkVertexInputAttributeDescription>& vertexAttribs) {
	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.pNext = nullptr;
	vertexInputInfo.flags = 0;
	vertexInputInfo.vertexBindingDescriptionCount = vertexBindingInfo ? 1 : 0;
	vertexInputInfo.pVertexBindingDescriptions = vertexBindingInfo;
	vertexInputInfo.vertexAttributeDescriptionCount = vertexAttribs.size();
	vertexInputInfo.pVertexAttributeDescriptions = vertexAttribs.data();
	return vertexInputInfo;
}

inline auto getInputAssemblyInfoTriangleList() {
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.pNext = nullptr;
	inputAssemblyInfo.flags = 0;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
	return inputAssemblyInfo;
}

inline auto getInputAssemblyInfoTriangleStrip() {
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.pNext = nullptr;
	inputAssemblyInfo.flags = 0;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
	return inputAssemblyInfo;
}

inline auto getViewportFullscreen() {
	VkViewport viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = windowSize.x;
	viewport.height = windowSize.y;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;
	return viewport;
}

inline auto getScissorFullscreen() {
	VkRect2D scissor;
	scissor.offset = {0, 0};
	scissor.extent = {windowSize.x, windowSize.y};
	return scissor;
}

inline auto getViewportStateInfo(const VkViewport& viewport, const VkRect2D scissor) {
	VkPipelineViewportStateCreateInfo viewportStateInfo;
	viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateInfo.pNext = nullptr;
	viewportStateInfo.flags = 0;
	viewportStateInfo.viewportCount = 1;
	viewportStateInfo.pViewports = &viewport;
	viewportStateInfo.scissorCount = 1;
	viewportStateInfo.pScissors = &scissor;
	return viewportStateInfo;
}

inline auto getRasterizationInfo() {
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationInfo.pNext = nullptr;
	rasterizationInfo.flags = 0;
	rasterizationInfo.depthClampEnable = VK_FALSE;
	rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizationInfo.depthBiasEnable = VK_FALSE;
	rasterizationInfo.depthBiasConstantFactor = 0.f;
	rasterizationInfo.depthBiasClamp = 0.f;
	rasterizationInfo.depthBiasSlopeFactor = 0.f;
	rasterizationInfo.lineWidth = 1.f;
	return rasterizationInfo;
}

inline auto getMultisampleInfo() {
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleInfo.pNext = nullptr;
	multisampleInfo.flags = 0;
	multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleInfo.sampleShadingEnable = VK_FALSE;
	multisampleInfo.minSampleShading = 1.f;
	multisampleInfo.pSampleMask = nullptr;
	multisampleInfo.alphaToCoverageEnable = VK_FALSE;
	multisampleInfo.alphaToOneEnable = VK_FALSE;
	return multisampleInfo;
}

inline auto getColorBlendAttachmentAdditive() {
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	return colorBlendAttachment;
}

inline auto getColorBlendInfo(const std::vector<VkPipelineColorBlendAttachmentState>& colorBlendAttachments) {
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendInfo.pNext = nullptr;
	colorBlendInfo.flags = 0;
	colorBlendInfo.logicOpEnable = VK_FALSE;
	colorBlendInfo.logicOp = VK_LOGIC_OP_NO_OP;
	colorBlendInfo.attachmentCount = colorBlendAttachments.size();
	colorBlendInfo.pAttachments = colorBlendAttachments.data();
	colorBlendInfo.blendConstants[0] = 0.f;
	colorBlendInfo.blendConstants[1] = 0.f;
	colorBlendInfo.blendConstants[2] = 0.f;
	colorBlendInfo.blendConstants[3] = 0.f;
	return colorBlendInfo;
}

inline auto getDynamicStateInfo(const std::vector<VkDynamicState>& dynamicStates) {
	VkPipelineDynamicStateCreateInfo dynamicStateInfo;
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.pNext = nullptr;
	dynamicStateInfo.flags = 0;
	dynamicStateInfo.dynamicStateCount = dynamicStates.size();
	dynamicStateInfo.pDynamicStates = dynamicStates.data();
	return dynamicStateInfo;
}

inline auto getDepthStencilInfoDisabled() {
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilInfo.pNext = nullptr;
	depthStencilInfo.flags = 0;
	depthStencilInfo.depthTestEnable = VK_FALSE;
	depthStencilInfo.depthWriteEnable = VK_FALSE;
	depthStencilInfo.depthCompareOp = VK_COMPARE_OP_NEVER;
	depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	depthStencilInfo.stencilTestEnable = VK_FALSE;
	depthStencilInfo.front = {};
	depthStencilInfo.back = {};
	depthStencilInfo.minDepthBounds = 0.0f;
	depthStencilInfo.maxDepthBounds = 1.0f;
	return depthStencilInfo;
}

inline auto getDepthStencilInfoDepthEnabled() {
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilInfo.pNext = nullptr;
	depthStencilInfo.flags = 0;
	depthStencilInfo.depthTestEnable = VK_TRUE;
	depthStencilInfo.depthWriteEnable = VK_TRUE;
	depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	depthStencilInfo.stencilTestEnable = VK_FALSE;
	depthStencilInfo.front = {};
	depthStencilInfo.back = {};
	depthStencilInfo.minDepthBounds = 0.0f;
	depthStencilInfo.maxDepthBounds = 1.0f;
	return depthStencilInfo;
}

inline auto getGraphicsPipelineInfo(
		const std::vector<VkPipelineShaderStageCreateInfo>& shaderStageInfos,
		const VkPipelineVertexInputStateCreateInfo& vertexInputInfo,
		const VkPipelineInputAssemblyStateCreateInfo& inputAssemblyInfo,
		const VkPipelineViewportStateCreateInfo& viewportStateInfo,
		const VkPipelineRasterizationStateCreateInfo& rasterizationInfo,
		const VkPipelineMultisampleStateCreateInfo& multisampleInfo,
		const VkPipelineDepthStencilStateCreateInfo& depthStencilInfo,
		const VkPipelineColorBlendStateCreateInfo& colorBlendInfo,
		const VkPipelineDynamicStateCreateInfo& dynamicStateInfo,
		const VkPipelineLayout pipelineLayout,
		const VkRenderPass renderPass,
		std::size_t subpassIndex) {
	VkGraphicsPipelineCreateInfo graphicsPipelineInfo;
	graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineInfo.pNext = nullptr;
	graphicsPipelineInfo.flags = 0;
	graphicsPipelineInfo.stageCount = shaderStageInfos.size();
	graphicsPipelineInfo.pStages = shaderStageInfos.data();
	graphicsPipelineInfo.pVertexInputState = &vertexInputInfo;
	graphicsPipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
	graphicsPipelineInfo.pTessellationState = nullptr;
	graphicsPipelineInfo.pViewportState = &viewportStateInfo;
	graphicsPipelineInfo.pRasterizationState = &rasterizationInfo;
	graphicsPipelineInfo.pMultisampleState = &multisampleInfo;
	graphicsPipelineInfo.pDepthStencilState = &depthStencilInfo;
	graphicsPipelineInfo.pColorBlendState = &colorBlendInfo;
	graphicsPipelineInfo.pDynamicState = &dynamicStateInfo;
	graphicsPipelineInfo.layout = pipelineLayout;
	graphicsPipelineInfo.renderPass = renderPass;
	graphicsPipelineInfo.subpass = subpassIndex;
	graphicsPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	graphicsPipelineInfo.basePipelineIndex = -1;
	return graphicsPipelineInfo;
}

class GraphicsPipelineRaii {
public:
	GraphicsPipelineRaii(VkDevice device, VkPipelineLayout layout, VkPipeline pipeline, VkDescriptorSetLayout descriptorLayout, VkDescriptorPool desccriptorPool, VkDescriptorSet descriptorSet);
	GraphicsPipelineRaii(const GraphicsPipelineRaii&) = delete;
	GraphicsPipelineRaii(GraphicsPipelineRaii&&) = default;
	~GraphicsPipelineRaii();

	GraphicsPipelineRaii& operator= (const GraphicsPipelineRaii&) = delete;
	GraphicsPipelineRaii& operator= (GraphicsPipelineRaii&&) = default;

	VkDevice device = VK_NULL_HANDLE;
	// Resource<VkShaderModule> shaderVert = VK_NULL_HANDLE;
	// Resource<VkShaderModule> shaderFrag = VK_NULL_HANDLE;
	Resource<VkPipelineLayout> layout = VK_NULL_HANDLE;
	Resource<VkPipeline> pipeline = VK_NULL_HANDLE;
	Resource<VkDescriptorSetLayout> descriptorLayout = VK_NULL_HANDLE;
	Resource<VkDescriptorPool> descriptorPool = VK_NULL_HANDLE;
	Resource<VkDescriptorSet> descriptorSet = VK_NULL_HANDLE;
};

} // namespace GraphicsPipeline
