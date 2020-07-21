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

auto createVertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, const PolygonPipeline::Vertices& vertices)
{
	auto size = sizeof(PolygonPipeline::Vertex) * vertices.size();
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

auto fillCommandBuffers(const std::vector<VkCommandBuffer>& commandBuffers, const std::vector<VkFramebuffer>& framebuffers, VkRenderPass renderPass, VkPipeline pipeline, VkPipelineLayout pipelineLayout,  const std::vector<VkBuffer>& buffers, const std::vector<VkDescriptorSet>& descriptorSets, std::size_t verticesCount)
{
	VkCommandBufferBeginInfo beginInfo;
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.pNext = nullptr;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
  beginInfo.pInheritanceInfo = nullptr;

	for (std::size_t i = 0; i < commandBuffers.size(); ++i) {
		error << vkBeginCommandBuffer(commandBuffers[i], &beginInfo);

		VkRenderPassBeginInfo renderPassBeginInfo;
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.framebuffer = framebuffers[i];
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

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffers[i], 0, buffers.size(), buffers.data(), offsets);
		vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr);
		vkCmdSetViewport(commandBuffers[i], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[i], 0, 1, &scissor);
		vkCmdDraw(commandBuffers[i], verticesCount, 1, 0, 0);
		vkCmdEndRenderPass(commandBuffers[i]);

		error << vkEndCommandBuffer(commandBuffers[i]);
	}
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

auto render(VkDevice device, VkSwapchainKHR swapchain, VkQueue queue, const std::vector<VkCommandBuffer>& commandBuffers, VkSemaphore semaphoreImageAvailable, VkSemaphore semaphoreRenderingDone)
{
	std::uint32_t imageIndex;
	error << vkAcquireNextImageKHR(device, swapchain, std::numeric_limits<std::uint64_t>::max(), semaphoreImageAvailable, VK_NULL_HANDLE, &imageIndex);

	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &semaphoreImageAvailable;
	VkPipelineStageFlags waitStages[] {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
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

// auto recreateSwapChain(VkDevice device, VkSurfaceKHR surface, VkSwapchainKHR* swapchain, std::vector<VkImageView>* imageViews, VkRenderPass* renderPass, std::vector<VkFramebuffer>* frameBuffers, VkCommandPool* commandPool, std::vector<VkCommandBuffer>* commandBuffers)
// {
// 	vkFreeCommandBuffers(device, *commandPool, imageViews->size(), commandBuffers->data()); // implicit when pool gets destroyed
// 	vkDestroyCommandPool(device, *commandPool, nullptr);
// 	for (const auto& framebuffer : *frameBuffers)
// 		vkDestroyFramebuffer(device, framebuffer, nullptr);
// 	vkDestroyRenderPass(device, *renderPass, nullptr);
// 	for (const auto& imageView : *imageViews)
// 		vkDestroyImageView(device, imageView, nullptr);
// 	vkDestroySwapchainKHR(device, *swapchain, nullptr);
//
// 	*swapchain = createSwapchain(device, surface);
// 	*imageViews = createSwapchainImageViews(device, *swapchain);
// 	*renderPass = createRenderPass(device);
// 	*frameBuffers = createFrameBuffers(device, *imageViews, *renderPass);
// 	*commandPool = createCommandPool(device);
// 	*commandBuffers = createCommandBuffers(device, *commandPool, imageViews->size());
// }

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	std::cout << std::boolalpha;
	auto startTime = cloc::now();
	auto sleepTime = 0ms;

	initGlfw();

	auto instance = Instance();
	auto [pipeline, uniformBuffers, uniformTextures] = PolygonPipeline::createPipeline(instance);
	auto* uboUniform = uniformBuffers[0];

	// auto [vertexBufferMemory, vertexBuffer, verticesCount] = createVertexBuffer(device, physicalDevice, commandPool, queue);
	auto [semaphoreImageAvailable, semaphoreRenderingDone] = createSemaphores(instance.device);

	auto renderer = PolygonRenderer();

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
		uboUniform->update(ubo);

		renderer.drawCircle(glm::vec2(0), 1.f, glm::vec3(1));
		renderer.drawTriangle(
			glm::vec2(-1, -1), glm::vec3(1), glm::vec2(0, 0),
			glm::vec2(1, -1), glm::vec3(1), glm::vec2(1, 0),
			glm::vec2(1, 1), glm::vec3(1), glm::vec2(1, 1));
		auto vertices = renderer.flush();
		assert(vertices.size() > 0);
		auto [vertexBufferMemory, vertexBuffer, verticesCount] = createVertexBuffer(instance.device, instance.physicalDevice, instance.commandPool, instance.queue, vertices);

		fillCommandBuffers(instance.commandBuffers, instance.frameBuffers, instance.renderPass, pipeline->pipeline, pipeline->layout, {vertexBuffer}, pipeline->descriptorSets, verticesCount);
		render(instance.device, instance.swapchain, instance.queue, instance.commandBuffers, semaphoreImageAvailable, semaphoreRenderingDone);
		vkDeviceWaitIdle(instance.device);
		vkFreeMemory(instance.device, vertexBufferMemory, nullptr);
		vkDestroyBuffer(instance.device, vertexBuffer, nullptr);

		glfwPollEvents();
		if (glfwGetKey(instance.window, GLFW_KEY_Q))
			glfwSetWindowShouldClose(instance.window, true);
		while(glfwGetKey(instance.window, GLFW_KEY_W)) {
			auto duration = 10ms;
			sleepTime += duration;
			std::this_thread::sleep_for(duration);
			glfwPollEvents();
		}
		std::this_thread::sleep_for(10ms);
	}

	// clean up phase
	vkDeviceWaitIdle(instance.device);

	vkDestroySemaphore(instance.device, semaphoreRenderingDone, nullptr);
	vkDestroySemaphore(instance.device, semaphoreImageAvailable, nullptr);

	glfwTerminate();

	return 0;
}
