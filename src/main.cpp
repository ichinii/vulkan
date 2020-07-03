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

auto updateUniformBuffer(VkDevice device, const std::vector<VkDeviceMemory>& memory, std::size_t imageIndex, std::chrono::milliseconds elapsedTime)
{
	auto ubo = UBO { glm::mat4{1.f} };
	ubo.mvp = ubo.mvp * glm::perspective(70.f, static_cast<float>(windowSize.x) / windowSize.y, .1f, 100.f);
	ubo.mvp = ubo.mvp * glm::lookAt(glm::vec3(0, 0, 2.), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	ubo.mvp = glm::translate(ubo.mvp, glm::vec3(.2 + .2f * glm::sin(elapsedTime.count() / 2621.87f), 0, 0));
	ubo.mvp = glm::scale(ubo.mvp, glm::vec3(.8, .8, 1));
	ubo.mvp = glm::rotate(ubo.mvp, .1f * glm::sin(elapsedTime.count() / 1000.f), glm::vec3(0, 0, 1));
	void* data;
	vkMapMemory(device, memory[imageIndex], 0, sizeof(ubo), 0, &data);
	std::memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(device, memory[imageIndex]);
}

auto updateUniformBuffers(VkDevice device, const std::vector<VkDeviceMemory>& memories, std::chrono::milliseconds elapsedTime)
{
	for (std::size_t i = 0; i < memories.size(); ++i)
		updateUniformBuffer(device, memories, i, elapsedTime);
}

auto updateDescriptors(VkDevice device, const std::vector<VkBuffer>& uniformBuffers, const std::vector<VkImageView> textureViews, VkSampler textureSampler, const std::vector<VkImageView> textureViews2, VkSampler textureSampler2, std::vector<VkDescriptorSet>& descriptorSets, std::size_t count)
{
	for (std::size_t i = 0; i < count; ++i) {
		VkDescriptorBufferInfo bufferInfo;
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UBO);

		VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = textureViews[i];
    imageInfo.sampler = textureSampler;

		VkDescriptorImageInfo imageInfo2{};
    imageInfo2.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo2.imageView = textureViews2[i];
    imageInfo2.sampler = textureSampler2;

		auto descriptorWrites = std::array<VkWriteDescriptorSet, 3>{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].pNext = nullptr;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].pImageInfo = nullptr;
		descriptorWrites[0].pBufferInfo = &bufferInfo;
		descriptorWrites[0].pTexelBufferView = nullptr;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[2].dstSet = descriptorSets[i];
		descriptorWrites[2].dstBinding = 2;
		descriptorWrites[2].dstArrayElement = 0;
		descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[2].descriptorCount = 1;
		descriptorWrites[2].pImageInfo = &imageInfo2;

		vkUpdateDescriptorSets(device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
	}
}

auto createVertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, const std::vector<Vertex>& vertices)
{
	auto size = sizeof(Vertex) * vertices.size();
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

auto fillCommandBuffers(const std::vector<VkCommandBuffer>& commandBuffers, const std::vector<VkFramebuffer>& framebuffers, VkRenderPass renderPass, VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout,  const std::vector<VkBuffer>& buffers, const std::vector<VkDescriptorSet>& descriptorSets, std::size_t verticesCount)
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
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
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
	// auto instance = createInstance();
	// auto window = createWindow();
	// auto physicalDevice = getPhysicalDevice(instance);
	// auto device = createDevice(physicalDevice);
	// auto surface = createSurface(instance, window, physicalDevice);
  //
	// auto swapchain = createSwapchain(device, surface);
	// auto imageViews = createSwapchainImageViews(device, swapchain);
	// auto renderPass = createRenderPass(device);
	// auto frameBuffers = createFrameBuffers(device, imageViews, renderPass);
	// auto commandPool = createCommandPool(device);
	// auto commandBuffers = createCommandBuffers(device, commandPool, imageViews.size());

	auto pipeline = Pipeline(instance.physicalDevice, instance.device, instance.renderPass, instance.imageViews.size());
	// // pipeline descriptors
	// auto [uniformMemories, uniformBuffers] = createUniformBuffers(physicalDevice, device, imageViews.size());
	// auto descriptorLayout = createDescriptorSetLayout(device);
	// auto descriptorLayouts = std::vector<VkDescriptorSetLayout>(imageViews.size(), descriptorLayout);
	// // pipeline
	// auto [shaderVert, shaderFrag] = createShaders(device);
	// auto pipelineLayout = createPipelineLayout(device, descriptorLayouts);
	// auto graphicsPipeline = createPipeline(device, renderPass, pipelineLayout, shaderVert, shaderFrag);

	// textures
	auto texture0 = Texture::fromGenerator(instance, glm::uvec2(8, 8), [] (glm::vec2 uv) {
		uv = glm::abs(uv);
		return glm::vec4(uv.y, 0, uv.x, 1);
	});
	auto texture1 = Texture::fromGenerator(instance, glm::uvec2(64, 64), [] (glm::vec2 uv) {
		return glm::vec4(0, glm::sin(uv.x * 6.f + uv.y * 4.f), 0, 1);
	});

	auto descriptorPool = createDescriptorPool(instance.device, instance.imageViews.size());
	auto descriptorSets = createDescriptorSets(instance.device, pipeline.descriptorLayouts, descriptorPool, instance.imageViews.size());
	// actually bind uniform and sampler to an index
	updateDescriptors(instance.device, pipeline.uniformBuffers, texture0.views, texture0.sampler, texture1.views, texture1.sampler, descriptorSets, instance.imageViews.size());

	// auto [vertexBufferMemory, vertexBuffer, verticesCount] = createVertexBuffer(device, physicalDevice, commandPool, queue);
	auto [semaphoreImageAvailable, semaphoreRenderingDone] = createSemaphores(instance.device);

	auto renderer = PolygonRenderer();

	// we initialzed vulkan (yay)
	std::cout << "hello vulkan" << std::endl;

	while (!glfwWindowShouldClose(instance.window)) {
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(cloc::now() - startTime) - sleepTime;

		renderer.drawCircle(glm::vec2(0), 1.f, glm::vec3(1));
		renderer.drawTriangle(
			glm::vec2(-1, -1), glm::vec3(1), glm::vec2(0, 0),
			glm::vec2(1, -1), glm::vec3(1), glm::vec2(1, 0),
			glm::vec2(1, 1), glm::vec3(1), glm::vec2(1, 1));
		auto vertices = renderer.flush();
		assert(vertices.size() > 0);
		auto [vertexBufferMemory, vertexBuffer, verticesCount] = createVertexBuffer(instance.device, instance.physicalDevice, instance.commandPool, instance.queue, vertices);
		fillCommandBuffers(instance.commandBuffers, instance.frameBuffers, instance.renderPass, pipeline.pipeline, pipeline.layout, {vertexBuffer}, descriptorSets, verticesCount);
		updateUniformBuffers(instance.device, pipeline.uniformMemories, elapsedTime);
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

	// destroy vulkan
	vkDeviceWaitIdle(instance.device);

	vkDestroySemaphore(instance.device, semaphoreRenderingDone, nullptr);
	vkDestroySemaphore(instance.device, semaphoreImageAvailable, nullptr);

	// vkFreeDescriptorSets(device, descriptorPool, descriptorSets.size(), descriptorSets.data());
	vkDestroyDescriptorPool(instance.device, descriptorPool, nullptr);

	glfwTerminate();

	return 0;
}
