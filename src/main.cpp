#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "log.h"
#include "graphics.h"
#include "instance.h"
#include "pipeline.h"
#include "deferredgeometrypipeline.h"
#include "deferredlightingpipeline.h"
#include "texture.h"
#include "polygonrenderer.h"
#include "shader.h"

using namespace std::chrono_literals;
using cloc = std::chrono::steady_clock;

void initGlfw()
{
	assert(glfwInit() == GLFW_TRUE);
}

auto copyBuffer(VkDevice device, VkBuffer src, VkBuffer dst, std::size_t size, VkCommandPool commandPool, VkQueue queue) {
	auto commandBuffer = beginSingleTimeCommands(device, commandPool);

	VkBufferCopy copy{};
	copy.size = size;
	vkCmdCopyBuffer(commandBuffer, src, dst, 1, &copy);

	endSingleTimeCommands(device, commandBuffer, commandPool, queue);
}

template <typename T>
auto createVertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, const std::vector<T>& vertices)
{
	auto size = sizeof(T) * vertices.size();
	assert(size > 0);

	auto [stagingMemory, stagingBuffer] = createBuffer(physicalDevice, device, size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(device, stagingMemory, 0, size, 0, &data);
	std::memcpy(data, vertices.data(), size);
	vkUnmapMemory(device, stagingMemory);
	// vkFlushMappedMemoryRanges() // only needed if memory is not coherent

	auto [memory, buffer] = createBuffer(physicalDevice, device, size,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	copyBuffer(device, stagingBuffer, buffer, size, commandPool, queue);

	vkFreeMemory(device, stagingMemory, nullptr);
	vkDestroyBuffer(device, stagingBuffer, nullptr);

	return std::make_tuple(memory, buffer, vertices.size());
}

auto fillCommandBuffer(
		VkCommandBuffer commandBuffer,
		VkFramebuffer framebuffer,
		VkRenderPass renderPass,
		const GraphicsPipeline::GraphicsPipelineRaii& geometryPipeline,
		const GraphicsPipeline::GraphicsPipelineRaii& lightingPipeline,
		const std::vector<VkBuffer>& vertexBuffers,
		std::size_t verticesCount)
{
	VkCommandBufferBeginInfo beginInfo;
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = nullptr;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	beginInfo.pInheritanceInfo = nullptr;

	error << vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkRenderPassBeginInfo renderPassBeginInfo;
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.pNext = nullptr;
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.framebuffer = framebuffer;
	renderPassBeginInfo.renderArea.offset = {0, 0};
	renderPassBeginInfo.renderArea.extent = {windowSize.x, windowSize.y};

	VkClearValue colorClearValue = {{{0, 0, 0, 0}}};
	VkClearValue depthClearValue = {{{1.0f, .0f}}};
	VkClearValue albedoClearValue = {{{.01f, .06f, .1f, 0.f}}};
	VkClearValue normalClearValue = {{{0, 0, 0, 0}}};
	VkClearValue clearValues[4] {
		colorClearValue,
		depthClearValue,
		albedoClearValue,
		normalClearValue,
	};
	renderPassBeginInfo.clearValueCount = 4;
	renderPassBeginInfo.pClearValues = clearValues;

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

	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, geometryPipeline.pipeline);
	auto offsets = std::vector<VkDeviceSize>(vertexBuffers.size(), 0);
	vkCmdBindVertexBuffers(commandBuffer, 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, geometryPipeline.layout, 0, 1, &geometryPipeline.descriptorSet, 0, nullptr);
	vkCmdDraw(commandBuffer, verticesCount, 1, 0, 0);

	vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, lightingPipeline.pipeline);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, lightingPipeline.layout, 0, 1, &lightingPipeline.descriptorSet, 0, nullptr);
	vkCmdDraw(commandBuffer, 4, 1, 0, 0);
	
	vkCmdEndRenderPass(commandBuffer);

	error << vkEndCommandBuffer(commandBuffer);
}

auto createSemaphores(VkDevice device)
{
	VkSemaphoreCreateInfo semaphoreInfo;
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreInfo.pNext = nullptr;
	semaphoreInfo.flags = 0;

	VkSemaphore semaphoreImageAvailable;
	error << vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphoreImageAvailable);
	VkSemaphore semaphoreRenderingDone;
	error << vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphoreRenderingDone);

	return std::make_tuple(semaphoreImageAvailable, semaphoreRenderingDone);
}

auto acquireNextImage(VkDevice device, VkSwapchainKHR swapchain, VkSemaphore semaphoreImageAvailable)
{
	std::uint32_t imageIndex;
	error << vkAcquireNextImageKHR(device, swapchain, std::numeric_limits<std::uint64_t>::max(), semaphoreImageAvailable, VK_NULL_HANDLE, &imageIndex);

	return imageIndex;
}

auto render(VkSwapchainKHR swapchain, VkQueue queue, VkCommandBuffer commandBuffer, VkSemaphore semaphoreImageAvailable, VkSemaphore semaphoreRenderingDone, std::uint32_t imageIndex)
{
	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &semaphoreImageAvailable;
	VkPipelineStageFlags waitStages[] {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &semaphoreRenderingDone;

	error << vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

	VkPresentInfoKHR presentInfo;
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &semaphoreRenderingDone;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchain;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	error << vkQueuePresentKHR(queue, &presentInfo);
}

auto createRenderPass(VkDevice device)
{
	VkAttachmentDescription colorAttachmentDescription;
	colorAttachmentDescription.flags = 0;
	colorAttachmentDescription.format = image_format;
	colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depthAttachmentDescription;
	depthAttachmentDescription.flags = 0;
	depthAttachmentDescription.format = VK_FORMAT_D32_SFLOAT;
	depthAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depthAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription albedoAttachmentDescription;
	albedoAttachmentDescription.flags = 0;
	albedoAttachmentDescription.format = VK_FORMAT_R8G8B8A8_SRGB;
	albedoAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	albedoAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	albedoAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	albedoAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	albedoAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	albedoAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	albedoAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription normalAttachmentDescription;
	normalAttachmentDescription.flags = 0;
	normalAttachmentDescription.format = VK_FORMAT_R8G8B8A8_SNORM;
	normalAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	normalAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	normalAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	normalAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	normalAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	normalAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	normalAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription attachmentDescriptions[4] {
		colorAttachmentDescription,
		depthAttachmentDescription,
		albedoAttachmentDescription,
		normalAttachmentDescription,
	};

	VkAttachmentReference colorAttachmentReference;
	colorAttachmentReference.attachment = 0;
	colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentReferenceOutput;
	depthAttachmentReferenceOutput.attachment = 1;
	depthAttachmentReferenceOutput.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentReferenceInput;
	depthAttachmentReferenceInput.attachment = 1;
	depthAttachmentReferenceInput.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkAttachmentReference albedoAttachmentReferenceOutput;
	albedoAttachmentReferenceOutput.attachment = 2;
	albedoAttachmentReferenceOutput.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference albedoAttachmentReferenceInput;
	albedoAttachmentReferenceInput.attachment = 2;
	albedoAttachmentReferenceInput.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkAttachmentReference normalAttachmentReferenceOutput;
	normalAttachmentReferenceOutput.attachment = 3;
	normalAttachmentReferenceOutput.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference normalAttachmentReferenceInput;
	normalAttachmentReferenceInput.attachment = 3;
	normalAttachmentReferenceInput.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkAttachmentReference colorAttachments[2] {
		albedoAttachmentReferenceOutput,
		normalAttachmentReferenceOutput,
	};

	VkSubpassDescription geometrySubpassDescription;
	geometrySubpassDescription.flags = 0;
	geometrySubpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	geometrySubpassDescription.inputAttachmentCount = 0;
	geometrySubpassDescription.pInputAttachments = nullptr;
	geometrySubpassDescription.colorAttachmentCount = 2;
	geometrySubpassDescription.pColorAttachments = colorAttachments;
	geometrySubpassDescription.pResolveAttachments = nullptr;
	geometrySubpassDescription.pDepthStencilAttachment = &depthAttachmentReferenceOutput;
	geometrySubpassDescription.preserveAttachmentCount = 0;
	geometrySubpassDescription.pPreserveAttachments = nullptr;

	VkAttachmentReference inputAttachments[3] {
		albedoAttachmentReferenceInput,
		depthAttachmentReferenceInput,
		normalAttachmentReferenceInput,
	};

	VkSubpassDescription shadingSubpassDescription;
	shadingSubpassDescription.flags = 0;
	shadingSubpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	shadingSubpassDescription.inputAttachmentCount = 3;
	shadingSubpassDescription.pInputAttachments = inputAttachments;
	shadingSubpassDescription.colorAttachmentCount = 1;
	shadingSubpassDescription.pColorAttachments = &colorAttachmentReference;
	shadingSubpassDescription.pResolveAttachments = nullptr;
	shadingSubpassDescription.pDepthStencilAttachment = nullptr;
	shadingSubpassDescription.preserveAttachmentCount = 0;
	shadingSubpassDescription.pPreserveAttachments = nullptr;

	VkSubpassDescription subpassDescriptions[2] {
		geometrySubpassDescription,
		shadingSubpassDescription,
	};

	VkSubpassDependency subpassDependencies[3];

	subpassDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependencies[0].dstSubpass = 0;
	subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	subpassDependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	subpassDependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpassDependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	subpassDependencies[1].srcSubpass = 0;
	subpassDependencies[1].dstSubpass = 1;
	subpassDependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpassDependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpassDependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	subpassDependencies[2].srcSubpass = 1;
	subpassDependencies[2].dstSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependencies[2].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependencies[2].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	subpassDependencies[2].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpassDependencies[2].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	subpassDependencies[2].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	VkRenderPassCreateInfo renderPassInfo;
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.pNext = nullptr;
	renderPassInfo.flags = 0;
	renderPassInfo.attachmentCount = 4;
	renderPassInfo.pAttachments = attachmentDescriptions;
	renderPassInfo.subpassCount = 2;
	renderPassInfo.pSubpasses = subpassDescriptions;
	renderPassInfo.dependencyCount = 3;
	renderPassInfo.pDependencies = subpassDependencies;

	VkRenderPass renderPass;
	error << vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass);
	return renderPass;
}

auto createDepthImages(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, const std::vector<VkImageView>& swapchainImages) {
	auto memories = std::vector<VkDeviceMemory>();
	auto images = std::vector<VkImage>();
	auto imageViews = std::vector<VkImageView>();
	for (auto i = 0u; i < swapchainImages.size(); ++i) {
		auto [memory, image] = createDepthImage(device, physicalDevice, windowSize);
		auto imageView = createDepthImageView(device, image);
		memories.push_back(memory);
		images.push_back(image);
		imageViews.push_back(imageView);
	}
	return std::make_tuple(memories, images, imageViews);
}

auto createAlbedoImages(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, const std::vector<VkImageView>& swapchainImages) {
	auto memories = std::vector<VkDeviceMemory>();
	auto images = std::vector<VkImage>();
	auto imageViews = std::vector<VkImageView>();
	for (auto i = 0u; i < swapchainImages.size(); ++i) {
		auto [memory, image] = createAlbedoImage(device, physicalDevice, windowSize);
		auto imageView = createAlbedoImageView(device, image);
		memories.push_back(memory);
		images.push_back(image);
		imageViews.push_back(imageView);
	}
	return std::make_tuple(memories, images, imageViews);
}

auto createNormalImages(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, const std::vector<VkImageView>& swapchainImages) {
	auto memories = std::vector<VkDeviceMemory>();
	auto images = std::vector<VkImage>();
	auto imageViews = std::vector<VkImageView>();
	for (auto i = 0u; i < swapchainImages.size(); ++i) {
		auto [memory, image] = createNormalImage(device, physicalDevice, windowSize);
		auto imageView = createNormalImageView(device, image);
		memories.push_back(memory);
		images.push_back(image);
		imageViews.push_back(imageView);
	}
	return std::make_tuple(memories, images, imageViews);
}

auto createFrameBuffers(VkDevice device, const std::vector<VkImageView>& swapchainImageViews, const std::vector<VkImageView>& depthImageViews, const std::vector<VkImageView>& albedoImageViews, const std::vector<VkImageView>& normalImageViews, VkRenderPass renderPass)
{
	auto framebuffers = std::vector<VkFramebuffer>(swapchainImageViews.size());
	for (std::size_t i = 0; i < swapchainImageViews.size(); ++i) {
		VkFramebufferCreateInfo framebufferInfo;
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.pNext = nullptr;
		framebufferInfo.flags = 0;
		framebufferInfo.renderPass = renderPass;

		VkImageView views[4] = {
			swapchainImageViews[i],
			depthImageViews[i],
			albedoImageViews[i],
			normalImageViews[i],
		};
		framebufferInfo.attachmentCount = 4;
		framebufferInfo.pAttachments = views;
		framebufferInfo.width = windowSize.x;
		framebufferInfo.height = windowSize.y;
		framebufferInfo.layers = 1;

		error << vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffers[i]);
	}
	return framebuffers;
}

auto run()
{
	std::cout << std::boolalpha;
	auto startTime = cloc::now();
	auto sleepTime = 0ms;

	auto instance = Instance();
	auto renderPass = createRenderPass(instance.device);
	auto [depthMemories, depthImages, depthImageViews] = createDepthImages(instance.device, instance.physicalDevice, instance.commandPool, instance.queue, instance.imageViews);
	auto [albedoMemories, albedoImages, albedoImageViews] = createAlbedoImages(instance.device, instance.physicalDevice, instance.commandPool, instance.queue, instance.imageViews);
	auto [normalMemories, normalImages, normalImageViews] = createNormalImages(instance.device, instance.physicalDevice, instance.commandPool, instance.queue, instance.imageViews);
	auto frameBuffers = createFrameBuffers(instance.device, instance.imageViews, depthImageViews, albedoImageViews, normalImageViews, renderPass);

	auto geometryShaderVert = createShader(instance.device, "res/deferred_geometry.vert.spv");
	auto geometryShaderFrag = createShader(instance.device, "res/deferred_geometry.frag.spv");
	auto geometryPipeline = DeferredGeometryPipeline::createDeferredGeometryPipelineRaii(instance.device, geometryShaderVert, geometryShaderFrag, renderPass, 0);
	vkDestroyShaderModule(instance.device, geometryShaderVert, nullptr);
	vkDestroyShaderModule(instance.device, geometryShaderFrag, nullptr);

	auto geometryUniforms = DeferredGeometryPipeline::createUniforms(instance);
	auto& geometryUboUniform = std::get<UniformBuffer>(geometryUniforms[0].buffer);
	GraphicsPipeline::updateDescriptor(instance.device, geometryPipeline.descriptorSet, geometryUniforms);

	auto lightingShaderVert = createShader(instance.device, "res/deferred_lighting.vert.spv");
	auto lightingShaderFrag = createShader(instance.device, "res/deferred_lighting.frag.spv");
	auto lightingPipeline = DeferredLightingPipeline::createDeferredLightingPipelineRaii(instance.device, lightingShaderVert, lightingShaderFrag, renderPass, 1);
	vkDestroyShaderModule(instance.device, lightingShaderVert, nullptr);
	vkDestroyShaderModule(instance.device, lightingShaderFrag, nullptr);

	auto lightingUniforms = DeferredLightingPipeline::createUniforms(instance);
	auto& lightingUboUniform = std::get<UniformBuffer>(lightingUniforms[0].buffer);

	// auto [vertexBufferMemory, vertexBuffer, verticesCount] = createVertexBuffer(device, physicalDevice, commandPool, queue);
	auto [semaphoreImageAvailable, semaphoreRenderingDone] = createSemaphores(instance.device);

	auto renderer = PolygonRenderer();
	auto z = 0.25f;
	renderer.drawCircle(glm::vec3(0, 0, 0), 1.f, glm::vec3(0.5));
	renderer.drawCircle(glm::vec3(0, 0, .5), .5f, glm::vec3(0.5));
	renderer.drawTriangle(
		glm::vec3(-1, -1, z), glm::vec3(.3), glm::vec2(0, 0),
		glm::vec3(1, -1, z), glm::vec3(.3), glm::vec2(1, 0),
		glm::vec3(1, 1, z), glm::vec3(.3), glm::vec2(1, 1));
	auto vertices = renderer.flush();
	assert(vertices.size() > 0);
	auto [vertexBufferMemory, vertexBuffer, verticesCount] = createVertexBuffer(instance.device, instance.physicalDevice, instance.commandPool, instance.queue, vertices);

	// we initialzed vulkan (yay)
	std::cout << "hello vulkan" << std::endl;

	while (!glfwWindowShouldClose(instance.window)) {
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(cloc::now() - startTime) - sleepTime;

		auto p = glm::perspective(glm::radians(60.f), static_cast<float>(windowSize.x) / windowSize.y, .1f, 10.f);
		// auto v = glm::lookAt(glm::vec3(glm::sin(glfwGetTime()), 1., 2.5), glm::vec3(0), glm::vec3(0, 1, 0));
		// auto v = glm::lookAt(glm::vec3(0, 0, 2. + glm::sin(glfwGetTime())), glm::vec3(0), glm::vec3(0, 1, 0));
		auto v = glm::lookAt(glm::vec3(glm::sin(glfwGetTime()), 0, 3), glm::vec3(0.001, 0, 0), glm::vec3(0, 1, 0));
		auto m = glm::mat4(1.f);
		m = glm::rotate(m, static_cast<float>(glfwGetTime()), glm::vec3(0, 1, 0));
		m = glm::translate(m, glm::vec3(0, glm::sin(glfwGetTime() * .71), 0));
		// m = glm::translate(m, glm::vec3(0, 0, glm::sin(glfwGetTime() * 1.732)));
		// 	* glm::scale(glm::mat4(1.f), glm::vec3(.8, .8, 1))
		// 	* glm::rotate(glm::mat4(1.f), .1f * glm::sin(elapsedTime.count() / 1000.f), glm::vec3(0, 0, 1));
		auto mvp = p * v * m;

		DeferredGeometryPipeline::Ubo geometryUbo {
			mvp,
			m,
		};
		geometryUboUniform.update(geometryUbo);

		DeferredLightingPipeline::Ubo lightingUbo {
			glm::inverse(p * v),
			windowSize,
		};
		lightingUboUniform.update(lightingUbo);

		auto imageIndex = acquireNextImage(instance.device, instance.swapchain, semaphoreImageAvailable);

		GraphicsPipeline::updateDescriptor(instance.device, lightingPipeline.descriptorSet, lightingUniforms, albedoImageViews[imageIndex], depthImageViews[imageIndex], normalImageViews[imageIndex]);

		fillCommandBuffer(instance.commandBuffer, frameBuffers[imageIndex], renderPass, geometryPipeline, lightingPipeline, {vertexBuffer}, verticesCount);

		render(instance.swapchain, instance.queue, instance.commandBuffer, semaphoreImageAvailable, semaphoreRenderingDone, imageIndex);

		// wait because we have no fence
		vkDeviceWaitIdle(instance.device);

		glfwPollEvents();
		if (glfwGetKey(instance.window, GLFW_KEY_Q))
			glfwSetWindowShouldClose(instance.window, true);
		while(glfwGetKey(instance.window, GLFW_KEY_W)) {
			auto duration = 10ms;
			sleepTime += duration;
			std::this_thread::sleep_for(duration);
			glfwPollEvents();
		}
	}

	// clean up phase
	vkDeviceWaitIdle(instance.device);

	vkFreeMemory(instance.device, vertexBufferMemory, nullptr);
	vkDestroyBuffer(instance.device, vertexBuffer, nullptr);

	vkDestroySemaphore(instance.device, semaphoreRenderingDone, nullptr);
	vkDestroySemaphore(instance.device, semaphoreImageAvailable, nullptr);

	for (const auto& imageView : depthImageViews)
		vkDestroyImageView(instance.device, imageView, nullptr);
	for (const auto& image : depthImages)
		vkDestroyImage(instance.device, image, nullptr);
	for (const auto& memory : depthMemories)
		vkFreeMemory(instance.device, memory, nullptr);
	for (const auto& imageView : albedoImageViews)
		vkDestroyImageView(instance.device, imageView, nullptr);
	for (const auto& image : albedoImages)
		vkDestroyImage(instance.device, image, nullptr);
	for (const auto& memory : albedoMemories)
		vkFreeMemory(instance.device, memory, nullptr);
	for (const auto& imageView : normalImageViews)
		vkDestroyImageView(instance.device, imageView, nullptr);
	for (const auto& image : normalImages)
		vkDestroyImage(instance.device, image, nullptr);
	for (const auto& memory : normalMemories)
		vkFreeMemory(instance.device, memory, nullptr);
	for (const auto& framebuffer : frameBuffers)
		vkDestroyFramebuffer(instance.device, framebuffer, nullptr);
	vkDestroyRenderPass(instance.device, renderPass, nullptr);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	initGlfw();
	run();
	glfwTerminate();

	return 0;
}
