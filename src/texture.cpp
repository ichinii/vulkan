#include "texture.h"
#include "log.h"
#include <cstring>

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

auto transitionImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout) {
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

	transitionImageLayout(device, commandPool, queue, textureImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	copyBufferToImage(device, commandPool, queue, stagingBuffer, textureImage, image.size);

	vkFreeMemory(device, stagingMemory, nullptr);
	vkDestroyBuffer(device, stagingBuffer, nullptr);

	transitionImageLayout(device, commandPool, queue, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

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

Texture Texture::fromGenerator(const Instance& instance, glm::uvec2 size, std::function<Texture::GeneratorF> f) {
	auto colors = std::vector<Image::color>(size.x * size.y);
	std::size_t index = 0;
	std::generate(colors.begin(), colors.end(), [&] () {
		auto uv = glm::vec2(index % size.x, index / size.x);
		uv /= size;
		uv = uv * 2.f - 1.f;
		++index;
		auto color = f(uv);
		return glm::u8vec4(glm::clamp(glm::ivec4(color * 255.f), glm::ivec4(0), glm::ivec4(255)));
	});
	return Texture::fromImage(instance, Image{colors, size});
}

Texture Texture::fromFile(const Instance& instance, std::filesystem::path filepath) {
	assert(false), (void)"unimplemented";
	return {};
}

Texture Texture::fromImage(const Instance& instance, Image image) {
	Texture texture;
	auto [memory, textureImage] = createTexture(instance.device, instance.physicalDevice, instance.commandPool, instance.queue, image);
	auto views = createImageViews(instance.device, textureImage, instance.imageViews.size());
	auto sampler = createImageSampler(instance.device);
	texture.device = instance.device;
	texture.memory = memory;
	texture.image = textureImage;
	texture.views = views;
	texture.sampler = sampler;
	return texture;
}

Texture::Texture(Texture&& other)
	: device(other.device)
	, views(std::move(other.views))
{
	std::swap(memory, other.memory);
	std::swap(image, other.image);
	std::swap(sampler, other.sampler);
}

Texture& Texture::operator= (Texture&& other)
{
	std::swap(device, other.device);
	std::swap(memory, other.memory);
	std::swap(image, other.image);
	std::swap(sampler, other.sampler);
	std::swap(views, other.views);
	return *this;
}

Texture::~Texture()
{
	vkDestroySampler(device, sampler, nullptr);
	for (auto view : views)
		vkDestroyImageView(device, view, nullptr);
	vkDestroyImage(device, image, nullptr);
	vkFreeMemory(device, memory, nullptr);
}
