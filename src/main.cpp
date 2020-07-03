#include <cassert>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include "graphics.h"
#include "instance.h"
#include "pipeline.h"
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

auto beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool) {
	VkCommandBufferAllocateInfo commandBufferInfo;
	commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferInfo.pNext = nullptr;
  commandBufferInfo.commandPool = commandPool;
  commandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  commandBufferInfo.commandBufferCount = 1;
	VkCommandBuffer commandBuffer;
	error << vkAllocateCommandBuffers(device, &commandBufferInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo;
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.pNext = nullptr;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  beginInfo.pInheritanceInfo = nullptr;

	error << vkBeginCommandBuffer(commandBuffer, &beginInfo);
	return commandBuffer;
}

auto endSingleTimeCommands(VkDevice device, VkCommandBuffer commandBuffer, VkCommandPool commandPool, VkQueue queue) {
	error << vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = nullptr;
	submitInfo.pWaitDstStageMask = nullptr;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = nullptr;

	error << vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(queue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

auto copyBuffer(VkDevice device, VkBuffer src, VkBuffer dst, std::size_t size, VkCommandPool commandPool, VkQueue queue) {
	auto commandBuffer = beginSingleTimeCommands(device, commandPool);

	VkBufferCopy copy{};
	copy.size = size;
	vkCmdCopyBuffer(commandBuffer, src, dst, 1, &copy);

	endSingleTimeCommands(device, commandBuffer, commandPool, queue);
}

auto transitionImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
	auto commandBuffer = beginSingleTimeCommands(device, commandPool);

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = 0;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	} else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	} else {
		std::abort();
	}

	vkCmdPipelineBarrier(
		commandBuffer,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	endSingleTimeCommands(device, commandBuffer, commandPool, queue);
}

auto copyBufferToImage(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkBuffer buffer, VkImage image, glm::uvec2 size) {
	auto commandBuffer = beginSingleTimeCommands(device, commandPool);

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = {0, 0, 0};
	region.imageExtent = {
			size.x,
			size.y,
			1
	};

	vkCmdCopyBufferToImage(
		commandBuffer,
		buffer,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);

	endSingleTimeCommands(device, commandBuffer, commandPool, queue);
}

struct Image {
	using color = glm::u8vec4;
	std::vector<color> colors;
	glm::uvec2 size;
};

auto createImage(int divisions) {
	Image image;
	image.size.x = divisions;
	image.size.y = image.size.x;
	int index = 0;
	std::generate_n(std::back_inserter(image.colors), image.size.x * image.size.y, [&] () {
		auto uv = glm::vec2(index % image.size.x, glm::floor(index / image.size.x));
		uv /= image.size;
		uv = uv * 2.f - 1.f;
		auto z = glm::clamp(glm::dot(uv, glm::normalize(glm::vec2(1, -1))), 0.f, 1.f);
		uv = uv * .5f + .5f;
		++index;

		return glm::u8vec4(uv.x * 255, uv.y * 255, z * 255, 255);
	});
	return image;
}

#define texture_format VK_FORMAT_R8G8B8A8_SRGB

auto createTextureImage(VkDevice device, VkPhysicalDevice physicalDevice, glm::vec2 size) {
	VkImage textureImage;
	VkDeviceMemory textureMemory;

	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = static_cast<uint32_t>(size.x);
	imageInfo.extent.height = static_cast<uint32_t>(size.y);
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = texture_format;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0; // Optional

	error << vkCreateImage(device, &imageInfo, nullptr, &textureImage);

	VkMemoryRequirements requirements;
	vkGetImageMemoryRequirements(device, textureImage, &requirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = requirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	error << vkAllocateMemory(device, &allocInfo, nullptr, &textureMemory);

	vkBindImageMemory(device, textureImage, textureMemory, 0);

	return std::make_tuple(textureMemory, textureImage);
}

auto createTexture(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, Image image) {
	auto size = static_cast<VkDeviceSize>(image.size.x * image.size.y * sizeof(Image::color));
	assert(size > 0);

	auto [stagingMemory, stagingBuffer] = createBuffer(physicalDevice, device, size,
		 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(device, stagingMemory, 0, size, 0, &data);
	std::memcpy(data, image.colors.data(), size);
	vkUnmapMemory(device, stagingMemory);

	auto [textureMemory, textureImage] = createTextureImage(device, physicalDevice, image.size);

	transitionImageLayout(device, commandPool, queue, textureImage, texture_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	copyBufferToImage(device, commandPool, queue, stagingBuffer, textureImage, image.size);

	vkFreeMemory(device, stagingMemory, nullptr);
	vkDestroyBuffer(device, stagingBuffer, nullptr);

	transitionImageLayout(device, commandPool, queue, textureImage, texture_format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	return std::make_tuple(textureMemory, textureImage);
}

auto createImageView(VkDevice device, VkImage image) {
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = texture_format;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView textureView;
	error << vkCreateImageView(device, &viewInfo, nullptr, &textureView);

	return textureView;
}

auto createImageViews(VkDevice device, VkImage image, std::size_t size) {
	auto swapchainImageViews = std::vector<VkImageView>();
	std::generate_n(std::back_inserter(swapchainImageViews), size, [=] {
		return createImageView(device, image);
	});
	return swapchainImageViews;
}

auto createImageSampler(VkDevice device) {
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_NEAREST;
	samplerInfo.minFilter = VK_FILTER_NEAREST;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.anisotropyEnable = VK_FALSE;
	samplerInfo.maxAnisotropy = 16.0f;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	VkSampler sampler;
	error << vkCreateSampler(device, &samplerInfo, nullptr, &sampler);
	return sampler;
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

auto getQueue(VkDevice device)
{
	VkQueue queue;
	vkGetDeviceQueue(device, 0, 0, &queue);
	return queue;
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

	auto queue = getQueue(instance.device);

	// textures
	auto [textureMemory, textureImage] = createTexture(instance.device, instance.physicalDevice, instance.commandPool, queue, createImage(8));
	auto textureViews = createImageViews(instance.device, textureImage, instance.imageViews.size());
	auto textureSampler = createImageSampler(instance.device);
	auto [textureMemory2, textureImage2] = createTexture(instance.device, instance.physicalDevice, instance.commandPool, queue, createImage(32));
	auto textureViews2 = createImageViews(instance.device, textureImage2, instance.imageViews.size());
	auto textureSampler2 = createImageSampler(instance.device);

	auto descriptorPool = createDescriptorPool(instance.device, instance.imageViews.size());
	auto descriptorSets = createDescriptorSets(instance.device, pipeline.descriptorLayouts, descriptorPool, instance.imageViews.size());
	// actually bind uniform and sampler to an index
	updateDescriptors(instance.device, pipeline.uniformBuffers, textureViews, textureSampler, textureViews2, textureSampler2, descriptorSets, instance.imageViews.size());

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
		auto [vertexBufferMemory, vertexBuffer, verticesCount] = createVertexBuffer(instance.device, instance.physicalDevice, instance.commandPool, queue, vertices);
		fillCommandBuffers(instance.commandBuffers, instance.frameBuffers, instance.renderPass, pipeline.pipeline, pipeline.layout, {vertexBuffer}, descriptorSets, verticesCount);
		updateUniformBuffers(instance.device, pipeline.uniformMemories, elapsedTime);
		render(instance.device, instance.swapchain, queue, instance.commandBuffers, semaphoreImageAvailable, semaphoreRenderingDone);
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

	vkDestroySampler(instance.device, textureSampler, nullptr);
	for (auto textureView : textureViews)
		vkDestroyImageView(instance.device, textureView, nullptr);
	vkDestroyImage(instance.device, textureImage, nullptr);
	vkFreeMemory(instance.device, textureMemory, nullptr);

	vkDestroySampler(instance.device, textureSampler2, nullptr);
	for (auto textureView : textureViews2)
		vkDestroyImageView(instance.device, textureView, nullptr);
	vkDestroyImage(instance.device, textureImage2, nullptr);
	vkFreeMemory(instance.device, textureMemory2, nullptr);

	vkDestroySemaphore(instance.device, semaphoreRenderingDone, nullptr);
	vkDestroySemaphore(instance.device, semaphoreImageAvailable, nullptr);

	// vkFreeDescriptorSets(device, descriptorPool, descriptorSets.size(), descriptorSets.data());
	vkDestroyDescriptorPool(instance.device, descriptorPool, nullptr);

	glfwTerminate();

	return 0;
}
