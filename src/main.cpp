#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <cstring>
#include <algorithm>
#include "graphics.h"
#include "instance.h"
#include "pipeline.h"
#include "polygonpipeline.h"
#include "texture.h"
#include "polygonrenderer.h"
#include "shader.h"
#include "log.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

auto fillCommandBuffer(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer, VkRenderPass renderPass, VkPipeline pipeline, VkPipelineLayout pipelineLayout,	const std::vector<VkBuffer>& vertexBuffers, VkDescriptorSet& descriptorSet, std::size_t verticesCount)
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

	VkClearValue clearValue = {{{.01f, .06f, .1f, 1.f}}};
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = &clearValue;

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

	vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
	auto offsets = std::vector<VkDeviceSize>(vertexBuffers.size(), 0);
	vkCmdBindVertexBuffers(commandBuffer, 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	vkCmdDraw(commandBuffer, verticesCount, 1, 0, 0);
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

auto run()
{
	std::cout << std::boolalpha;
	auto startTime = cloc::now();
	auto sleepTime = 0ms;

	auto instance = Instance();
	auto pipeline = PolygonPipeline::createPipeline(instance);
	auto uniformInfos = pipeline->uniformInfos;
	auto uniforms = PolygonPipeline::createUniforms(instance);
	auto& uboUniform = std::get<UniformBuffer>(uniforms[0].buffer);

	pipeline->updateUniforms(uniforms);

	// auto [vertexBufferMemory, vertexBuffer, verticesCount] = createVertexBuffer(device, physicalDevice, commandPool, queue);
	auto [semaphoreImageAvailable, semaphoreRenderingDone] = createSemaphores(instance.device);

	auto renderer = PolygonRenderer();
	renderer.drawCircle(glm::vec2(0), 1.f, glm::vec3(1));
	renderer.drawTriangle(
		glm::vec2(-1, -1), glm::vec3(1), glm::vec2(0, 0),
		glm::vec2(1, -1), glm::vec3(1), glm::vec2(1, 0),
		glm::vec2(1, 1), glm::vec3(1), glm::vec2(1, 1));
	auto vertices = renderer.flush();
	assert(vertices.size() > 0);
	auto [vertexBufferMemory, vertexBuffer, verticesCount] = createVertexBuffer(instance.device, instance.physicalDevice, instance.commandPool, instance.queue, vertices);

	// we initialzed vulkan (yay)
	std::cout << "hello vulkan" << std::endl;

	while (!glfwWindowShouldClose(instance.window)) {
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(cloc::now() - startTime) - sleepTime;

		auto ubo = PolygonPipeline::Ubo();
		ubo.mvp = glm::mat4(1.f);
		ubo.mvp = ubo.mvp * glm::perspective(70.f, static_cast<float>(windowSize.x) / windowSize.y, .1f, 100.f);
		ubo.mvp = ubo.mvp * glm::lookAt(glm::vec3(0, 0, 2.), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		ubo.mvp = glm::translate(ubo.mvp, glm::vec3(.2 + .2f * glm::sin(elapsedTime.count() / 2621.87f), 0, 0));
		ubo.mvp = glm::scale(ubo.mvp, glm::vec3(.8, .8, 1));
		ubo.mvp = glm::rotate(ubo.mvp, .1f * glm::sin(elapsedTime.count() / 1000.f), glm::vec3(0, 0, 1));
		uboUniform.update(ubo);

		auto imageIndex = acquireNextImage(instance.device, instance.swapchain, semaphoreImageAvailable);
		fillCommandBuffer(instance.commandBuffer, instance.frameBuffers[imageIndex], instance.renderPass, pipeline->pipeline, pipeline->layout, {vertexBuffer}, pipeline->descriptorSet, verticesCount);
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
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	initGlfw();
	run();
	glfwTerminate();

	return 0;
}
