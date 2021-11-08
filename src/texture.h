#pragma once

#include <glm/glm.hpp>
#include "instance.h"
#include "resource.h"
#include "log.h"

#define texture_format VK_FORMAT_R8G8B8A8_SRGB

struct Image {
	using color = glm::u8vec4;
	std::vector<color> colors;
	glm::uvec2 size;
};

inline auto createImage(VkDevice device, VkPhysicalDevice physicalDevice, glm::uvec2 size) {
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

inline auto createDepthImage(VkDevice device, VkPhysicalDevice physicalDevice, glm::uvec2 size) {
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
	imageInfo.format = VK_FORMAT_D32_SFLOAT;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
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

inline auto createAlbedoImage(VkDevice device, VkPhysicalDevice physicalDevice, glm::uvec2 size) {
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
	imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
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

inline auto createNormalImage(VkDevice device, VkPhysicalDevice physicalDevice, glm::uvec2 size) {
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
	imageInfo.format = VK_FORMAT_R32G32B32A32_SFLOAT;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
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

inline auto createColorImageView(VkDevice device, VkImage image) {
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

inline auto createDepthImageView(VkDevice device, VkImage image) {
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = VK_FORMAT_D32_SFLOAT;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView textureView;
	error << vkCreateImageView(device, &viewInfo, nullptr, &textureView);

	return textureView;
}

inline auto createAlbedoImageView(VkDevice device, VkImage image) {
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView textureView;
	error << vkCreateImageView(device, &viewInfo, nullptr, &textureView);

	return textureView;
}

inline auto createNormalImageView(VkDevice device, VkImage image) {
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = VK_FORMAT_R32G32B32A32_SFLOAT;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView textureView;
	error << vkCreateImageView(device, &viewInfo, nullptr, &textureView);

	return textureView;
}

inline auto transitionImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout) {
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

inline auto copyBufferToImage(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkBuffer buffer, VkImage image, glm::uvec2 size) {
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

inline auto createTexture(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, Image image) {
	auto size = static_cast<VkDeviceSize>(image.size.x * image.size.y * sizeof(Image::color));
	assert(size > 0);

	auto [stagingMemory, stagingBuffer] = createBuffer(physicalDevice, device, size,
		 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(device, stagingMemory, 0, size, 0, &data);
	std::memcpy(data, image.colors.data(), size);
	vkUnmapMemory(device, stagingMemory);

	auto [textureMemory, textureImage] = createImage(device, physicalDevice, image.size);

	transitionImageLayout(device, commandPool, queue, textureImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	copyBufferToImage(device, commandPool, queue, stagingBuffer, textureImage, image.size);

	vkFreeMemory(device, stagingMemory, nullptr);
	vkDestroyBuffer(device, stagingBuffer, nullptr);

	transitionImageLayout(device, commandPool, queue, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	return std::make_tuple(textureMemory, textureImage);
}

inline auto createImageSampler(VkDevice device) {
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

struct Texture {
	Texture() = default;
	Texture(const Texture&) = delete;
	Texture(Texture&& other) = default;
	~Texture();

	Texture& operator= (const Texture&) = delete;
	Texture& operator= (Texture&& other) = default;

	// parameter uv from (-1, -1) to (1, 1)
	using GeneratorF = glm::vec4(glm::vec2 uv);
	static Texture fromGenerator(const Instance& instance, glm::uvec2 size, std::function<GeneratorF> f);
	static Texture fromFile(const Instance& instance, const char* filepath);
	static Texture fromImage(const Instance& instance, Image Image);

	// void update();

	VkDevice device;
	Resource<VkDeviceMemory> memory = VK_NULL_HANDLE;
	Resource<VkImage> image = VK_NULL_HANDLE;
	Resource<VkSampler> sampler = VK_NULL_HANDLE;
	Resource<VkImageView> view = VK_NULL_HANDLE;
};

using Textures = std::vector<Texture>;
