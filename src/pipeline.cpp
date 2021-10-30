#include "pipeline.h"

namespace GraphicsPipeline {

GraphicsPipelineRaii::GraphicsPipelineRaii(
		VkDevice device_,
		VkPipelineLayout layout_,
		VkPipeline pipeline_,
		VkDescriptorPool desccriptorPool_,
		VkDescriptorSetLayout descriptorLayout_,
		VkDescriptorSet descriptorSet_) :
	device(device_),
	layout(layout_),
	pipeline(pipeline_),
	descriptorPool(desccriptorPool_),
	descriptorLayout(descriptorLayout_),
	descriptorSet(descriptorSet_)
{
}

GraphicsPipelineRaii::~GraphicsPipelineRaii()
{
	// vkFreeDescriptorSets(device, descriptorPool, descriptorSets.size(), descriptorSets.data());
	vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	vkDestroyPipeline(device, pipeline, nullptr);
	vkDestroyPipelineLayout(device, layout, nullptr);
	vkDestroyDescriptorSetLayout(device, descriptorLayout, nullptr);
}

void GraphicsPipelineRaii::updateUniforms(const Uniforms& uniforms)
{
	updateDescriptor(device, uniforms, descriptorSet);
}

} // namespace GraphicsPipeline
