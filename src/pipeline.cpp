#include "pipeline.h"
#include "shader.h"
#include <tuple>

auto getUniformBindings(const Uniforms& uniforms) {
	auto bindings = std::vector<VkDescriptorSetLayoutBinding>(uniforms.size());
	for (std::size_t i = 0; i < uniforms.size(); ++i) {
		auto& uniform  = uniforms[i];
		auto& binding = bindings[i];
		binding.binding = uniform.binding;
		binding.descriptorType = uniform.type;
		binding.descriptorCount = 1;
		binding.stageFlags = uniform.shaderStage;
		binding.pImmutableSamplers = nullptr; // Optional
	}
	return bindings;
}

auto createDescriptorSetLayout(VkDevice device, const Uniforms& uniforms)
{
	auto bindings = getUniformBindings(uniforms);

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

auto createPipelineLayout(VkDevice device, const std::vector<VkDescriptorSetLayout>& descriptorLayouts)
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.pNext = nullptr;
  pipelineLayoutInfo.flags = 0;
  pipelineLayoutInfo.setLayoutCount = descriptorLayouts.size();
  pipelineLayoutInfo.pSetLayouts = descriptorLayouts.data();
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;

	VkPipelineLayout pipelineLayout;
	error << vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout);
	return pipelineLayout;
}

auto getAttributeBindings(const Attributes& attributes)
{
	auto bindings = std::vector<VkVertexInputAttributeDescription>(attributes.size());

	for (std::size_t i = 0; i < attributes.size(); ++i) {
		auto& binding = bindings[i];
		auto& attribute = attributes[i];

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

auto createPipeline(VkDevice device, VkRenderPass renderPass, VkPipelineLayout pipelineLayout, VkShaderModule shaderVert, VkShaderModule shaderFrag, const Attributes& attributes, std::size_t vertexSize)
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
	auto vertexAttribs = getAttributeBindings(attributes);

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

auto createDescriptorPool(VkDevice device, std::size_t size)
{
	auto poolSizes = std::vector<VkDescriptorPoolSize> {
		{.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = static_cast<uint32_t>(size)},
		{.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = static_cast<uint32_t>(size)},
		{.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = static_cast<uint32_t>(size)},
	};

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

auto createDescriptorSets(VkDevice device, const std::vector<VkDescriptorSetLayout>& layouts, VkDescriptorPool pool, std::size_t count)
{
	VkDescriptorSetAllocateInfo allocInfo;
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = count;
	allocInfo.pSetLayouts = layouts.data();

	auto sets = std::vector<VkDescriptorSet>(count);
	error << vkAllocateDescriptorSets(device, &allocInfo, sets.data());

	return sets;
}

auto updateDescriptors(
		VkDevice device,
		const Uniforms& uniforms,
		const std::vector<VkDescriptorSet>& descriptorSets,
		std::size_t count)
{
	for (std::size_t i = 0; i < count; ++i) {
		auto bufferInfos = std::vector<VkDescriptorBufferInfo>(uniforms.size());
		auto imageInfos = std::vector<VkDescriptorImageInfo>(uniforms.size());

		for (std::size_t j = 0; j < uniforms.size(); ++j) {
			auto& uniform = uniforms[j];

			std::visit(overloaded{
				[&] (const UniformBuffer& buffer) {
					auto& bufferInfo = bufferInfos[j];
					bufferInfo.buffer = buffer.buffers[i];
					bufferInfo.offset = 0;
					bufferInfo.range = buffer.size;
				},
				[&] (const Texture& texture) {
					auto& imageInfo = imageInfos[j];
					imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfo.imageView = texture.views[i];
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
			write.dstSet = descriptorSets[i];
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
}

Uniform createUniform(int binding, VkShaderStageFlagBits stage, UniformBuffer buffer)
{
	auto type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	auto info = UniformInfo {binding, type, stage};
	return Uniform {info, std::move(buffer)};
}

Uniform createUniform(int binding, VkShaderStageFlagBits stage, Texture texture)
{
	auto type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	auto info = UniformInfo {binding, type, stage};
	return Uniform {info, std::move(texture)};
}

std::vector<UniformBuffer*> getUniformBuffers(Uniforms& uniforms)
{
	std::vector<UniformBuffer*> buffers;
	for (auto& uniform : uniforms) {
		std::visit(overloaded{
			[&] (UniformBuffer& buffer) {
				buffers.push_back(&buffer);
			}, [] (auto&) {}
		}, uniform.buffer);
	}
	return buffers;
}

std::vector<Texture*> getUniformTextures(Uniforms& uniforms)
{
	std::vector<Texture*> textures;
	for (auto& uniform : uniforms) {
		std::visit(overloaded{
			[&] (Texture& texture) {
				textures.push_back(&texture);
			}, [] (auto&) {}
		}, uniform.buffer);
	}
	return textures;
}

Pipeline::Pipeline(const Instance& instance, Uniforms&& uniforms, Attributes attributes, std::size_t vertexSize)
{
	device = instance.device;
	this->uniforms = std::move(uniforms);
	this->attributes = attributes;
	auto [shaderVert, shaderFrag] = createShaders(device);
	this->shaderVert = shaderVert;
	this->shaderFrag = shaderFrag;

	auto descriptorLayout = createDescriptorSetLayout(device, this->uniforms);
	descriptorLayouts = std::vector<VkDescriptorSetLayout>(instance.imageViews.size(), descriptorLayout);
	layout = createPipelineLayout(device, descriptorLayouts);
	pipeline = createPipeline(device, instance.renderPass, layout, shaderVert, shaderFrag, attributes, vertexSize);
	descriptorPool = createDescriptorPool(instance.device, instance.imageViews.size());
	descriptorSets = createDescriptorSets(instance.device, descriptorLayouts, descriptorPool, instance.imageViews.size());
	updateDescriptors(device, this->uniforms, descriptorSets, instance.imageViews.size());
}

Pipeline::~Pipeline()
{
	// vkFreeDescriptorSets(device, descriptorPool, descriptorSets.size(), descriptorSets.data());
	vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	vkDestroyPipeline(device, pipeline, nullptr);
	vkDestroyPipelineLayout(device, layout, nullptr);
	vkDestroyShaderModule(device, shaderVert, nullptr);
	vkDestroyShaderModule(device, shaderFrag, nullptr);
	vkDestroyDescriptorSetLayout(device, descriptorLayouts[0], nullptr);
}
