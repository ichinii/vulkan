#include "pipeline.h"

namespace GraphicsPipeline {

GraphicsPipelineRaii::GraphicsPipelineRaii(
		VkDevice device_,
		VkPipelineLayout layout_,
		VkPipeline pipeline_,
		VkDescriptorSetLayout descriptorLayout_,
		VkDescriptorPool desccriptorPool_,
		VkDescriptorSet descriptorSet_) :
	device(device_),
	layout(layout_),
	pipeline(pipeline_),
	descriptorLayout(descriptorLayout_),
	descriptorPool(desccriptorPool_),
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

} // namespace GraphicsPipeline
