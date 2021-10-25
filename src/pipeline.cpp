#include "pipeline.h"
#include "shader.h"
#include <tuple>

auto getUniformBindings(const UniformInfos& uniformInfos) {
	auto bindings = std::vector<VkDescriptorSetLayoutBinding>(uniformInfos.size());

	for (std::size_t i = 0; i < uniformInfos.size(); ++i) {
		auto& uniformInfo	= uniformInfos[i];
		auto& binding = bindings[i];
		binding.binding = uniformInfo.binding;
		binding.descriptorType = uniformInfo.type;
		binding.descriptorCount = 1;
		binding.stageFlags = uniformInfo.shaderStage;
		binding.pImmutableSamplers = nullptr; // Optional
	}

	return bindings;
}

auto createDescriptorSetLayout(VkDevice device, const UniformInfos& uniformInfos)
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

auto createShaders(VkDevice device)
{
	auto shaderCodeVert = readShaderFile("res/shader.vert.spv");
	auto shaderCodeFrag = readShaderFile("res/shader.frag.spv");

	auto shaderVert = createShader(device, shaderCodeVert);
	auto shaderFrag = createShader(device, shaderCodeFrag);

	return std::make_tuple(shaderVert, shaderFrag);
}

auto createPipelineLayout(VkDevice device, VkDescriptorSetLayout& descriptorLayout)
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

auto getAttributeBindings(const AttributeInfos& attributeInfos)
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

auto getBindingInfo(int vertexSize)
{
	VkVertexInputBindingDescription inputBinding;
	inputBinding.binding = 0;
	inputBinding.stride = vertexSize;
	inputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return inputBinding;
}

auto createDescriptorPool(VkDevice device, const UniformInfos& uniformInfos, std::size_t size)
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

auto createDescriptorSets(VkDevice device, VkDescriptorSetLayout layout, VkDescriptorPool pool)
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

auto updateDescriptor(
		VkDevice device,
		const Uniforms& uniforms,
		VkDescriptorSet& descriptorSet)
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

auto createPipeline(VkDevice device, VkRenderPass renderPass, VkPipelineLayout pipelineLayout, VkShaderModule shaderVert, VkShaderModule shaderFrag, const AttributeInfos& attributeInfos, std::size_t vertexSize)
{
	VkPipelineShaderStageCreateInfo shaderStageInfoVert;
	shaderStageInfoVert.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfoVert.pNext = nullptr;
	shaderStageInfoVert.flags = 0;
	shaderStageInfoVert.stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStageInfoVert.module = shaderVert;
	shaderStageInfoVert.pName = "main";
	shaderStageInfoVert.pSpecializationInfo = nullptr;

	VkPipelineShaderStageCreateInfo shaderStageInfoFrag;
	shaderStageInfoFrag.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfoFrag.pNext = nullptr;
	shaderStageInfoFrag.flags = 0;
	shaderStageInfoFrag.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStageInfoFrag.module = shaderFrag;
	shaderStageInfoFrag.pName = "main";
	shaderStageInfoFrag.pSpecializationInfo = nullptr;

	auto shaderStageInfos = std::vector<VkPipelineShaderStageCreateInfo> {shaderStageInfoVert, shaderStageInfoFrag};

	auto vertexBindingInfo = getBindingInfo(vertexSize);
	auto vertexAttribs = getAttributeBindings(attributeInfos);

	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.pNext = nullptr;
	vertexInputInfo.flags = 0;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &vertexBindingInfo;
	vertexInputInfo.vertexAttributeDescriptionCount = vertexAttribs.size();
	vertexInputInfo.pVertexAttributeDescriptions = vertexAttribs.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.pNext = nullptr;
	inputAssemblyInfo.flags = 0;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = windowSize.x;
	viewport.height = windowSize.y;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;

	VkRect2D scissor;
	scissor.offset = {0, 0};
	scissor.extent = {windowSize.x, windowSize.y};

	VkPipelineViewportStateCreateInfo viewportStateInfo;
	viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateInfo.pNext = nullptr;
	viewportStateInfo.flags = 0;
	viewportStateInfo.viewportCount = 1;
	viewportStateInfo.pViewports = &viewport;
	viewportStateInfo.scissorCount = 1;
	viewportStateInfo.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationInfo.pNext = nullptr;
	rasterizationInfo.flags = 0;
	rasterizationInfo.depthClampEnable = VK_FALSE;
	rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationInfo.cullMode = VK_CULL_MODE_NONE; // TODO: decide cull mode
	rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizationInfo.depthBiasEnable = VK_FALSE;
	rasterizationInfo.depthBiasConstantFactor = 0.f;
	rasterizationInfo.depthBiasClamp = 0.f;
	rasterizationInfo.depthBiasSlopeFactor = 0.f;
	rasterizationInfo.lineWidth = 1.f;

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

	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendInfo.pNext = nullptr;
	colorBlendInfo.flags = 0;
	colorBlendInfo.logicOpEnable = VK_FALSE;
	colorBlendInfo.logicOp = VK_LOGIC_OP_NO_OP;
	colorBlendInfo.attachmentCount = 1;
	colorBlendInfo.pAttachments = &colorBlendAttachment;
	colorBlendInfo.blendConstants[0] = 0.f;
	colorBlendInfo.blendConstants[1] = 0.f;
	colorBlendInfo.blendConstants[2] = 0.f;
	colorBlendInfo.blendConstants[3] = 0.f;

	auto dynamicStates = std::vector<VkDynamicState> {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicStateInfo;
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.pNext = nullptr;
	dynamicStateInfo.flags = 0;
	dynamicStateInfo.dynamicStateCount = dynamicStates.size();
	dynamicStateInfo.pDynamicStates = dynamicStates.data();

	VkGraphicsPipelineCreateInfo graphicsPipelineInfo;
	graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineInfo.pNext = nullptr;
	graphicsPipelineInfo.flags = 0;
	graphicsPipelineInfo.stageCount = 2;
	graphicsPipelineInfo.pStages = shaderStageInfos.data();
	graphicsPipelineInfo.pVertexInputState = &vertexInputInfo;
	graphicsPipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
	graphicsPipelineInfo.pTessellationState = nullptr;
	graphicsPipelineInfo.pViewportState = &viewportStateInfo;
	graphicsPipelineInfo.pRasterizationState = &rasterizationInfo;
	graphicsPipelineInfo.pMultisampleState = &multisampleInfo;
	graphicsPipelineInfo.pDepthStencilState = nullptr;
	graphicsPipelineInfo.pColorBlendState = &colorBlendInfo;
	graphicsPipelineInfo.pDynamicState = &dynamicStateInfo;
	graphicsPipelineInfo.layout = pipelineLayout;
	graphicsPipelineInfo.renderPass = renderPass;
	graphicsPipelineInfo.subpass = 0;
	graphicsPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	graphicsPipelineInfo.basePipelineIndex = -1;

	VkPipeline graphicsPipeline;
	error << vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &graphicsPipelineInfo, nullptr, &graphicsPipeline);

	return graphicsPipeline;
}

Pipeline::Pipeline(const Instance& instance, UniformInfos uniformInfos_, AttributeInfos attributeInfos_, std::size_t vertexSize)
{
	device = instance.device;
	uniformInfos = std::move(uniformInfos_);
	attributeInfos = std::move(attributeInfos_);
	auto [shaderVert_, shaderFrag_] = createShaders(device);
	shaderVert = shaderVert_;
	shaderFrag = shaderFrag_;

	descriptorLayout = createDescriptorSetLayout(device, uniformInfos);
	layout = createPipelineLayout(device, descriptorLayout);
	pipeline = createPipeline(device, instance.renderPass, layout, shaderVert, shaderFrag, attributeInfos, vertexSize);
	descriptorPool = createDescriptorPool(instance.device, uniformInfos, instance.imageViews.size());
	descriptorSet = createDescriptorSets(instance.device, descriptorLayout, descriptorPool);
}

Pipeline::~Pipeline()
{
	// vkFreeDescriptorSets(device, descriptorPool, descriptorSets.size(), descriptorSets.data());
	vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	vkDestroyPipeline(device, pipeline, nullptr);
	vkDestroyPipelineLayout(device, layout, nullptr);
	vkDestroyShaderModule(device, shaderVert, nullptr);
	vkDestroyShaderModule(device, shaderFrag, nullptr);
	vkDestroyDescriptorSetLayout(device, descriptorLayout, nullptr);
}

void Pipeline::updateUniforms(const Uniforms& uniforms)
{
	updateDescriptor(device, uniforms, descriptorSet);
}
