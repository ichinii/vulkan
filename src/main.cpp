#include <cassert>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include "graphics.h"
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

auto getLayerProperties()
{
	std::uint32_t layerCount;
	error << vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	auto layerProperties = std::vector<VkLayerProperties>(layerCount);
	error << vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.data());
	debug::dump("layer", layerProperties);
	return layerProperties;
}

auto getLayers()
{
	auto layerProperties = getLayerProperties();

	// TODO: choose layers nicely. maybe eliminate unavailable layers
	auto layers = std::vector<const char*>() = {
		"VK_LAYER_KHRONOS_validation",
		"VK_LAYER_MESA_overlay",
	};

	debug::dump_direct("choosing layers", layers);
	return layers;
}

auto getExtensionProperties()
{
	std::uint32_t extensionCount;
	error << vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	auto extensionProperties = std::vector<VkExtensionProperties>(extensionCount);
	error << vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data());

	debug::dump("extension", extensionProperties);
	return extensionProperties;
}

auto getExtensions()
{
	auto extensionProperties = getExtensionProperties();

	// TODO: choose extensions
	auto extensions = std::vector<const char*>() = {};

	std::uint32_t glfwExtensionCount;
	auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	for (std::size_t i = 0; i < glfwExtensionCount; ++i)
		extensions.push_back(glfwExtensions[i]);

	debug::dump_direct("choosing extensions", extensions);
	return extensions;
}

GLFWwindow* createWindow()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(windowSize.x, windowSize.y, "Vulkan", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	return window;
}

VkInstance createInstance()
{
	VkApplicationInfo appInfo;
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pNext = nullptr;
  appInfo.pApplicationName = "Generic Sauce Vulkan Application";
  appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
  appInfo.pEngineName = "Generic Sauce Vulkan Graphics Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
  appInfo.apiVersion = VK_VERSION_1_0;

	auto layers = getLayers();
	auto extensions = getExtensions();

	VkInstanceCreateInfo instanceInfo;
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext = nullptr;
	instanceInfo.flags = 0;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledLayerCount = layers.size();
	instanceInfo.ppEnabledLayerNames = layers.data();
	instanceInfo.enabledExtensionCount = extensions.size();
	instanceInfo.ppEnabledExtensionNames = extensions.data();

	VkInstance instance;
	error << vkCreateInstance(&instanceInfo, nullptr, &instance);

	return instance;
}

auto getPhysicalDevices(VkInstance instance)
{
	std::uint32_t physicalDeviceCount;
	error << vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
	auto physicalDevices = std::vector<VkPhysicalDevice>(physicalDeviceCount);
	error << vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
	debug::dump("physical devices", physicalDevices);
	return physicalDevices;
}

auto getPhysicalDevice(VkInstance instance)
{
	auto physicalDevices = getPhysicalDevices(instance);
	
	auto physicalDevice = physicalDevices[0]; // TODO: choose best device
	return physicalDevice;
}

auto createDevice(VkPhysicalDevice physicalDevice)
{
	VkDeviceQueueCreateInfo queueInfo;
	queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfo.pNext = nullptr;
  queueInfo.flags = 0;
  queueInfo.queueFamilyIndex = 0; // TODO: choose best family
  queueInfo.queueCount = 1; // TODO: check if amount is available
	auto queuePriorities = std::vector<float> {1.f};
  queueInfo.pQueuePriorities = queuePriorities.data();

	VkPhysicalDeviceFeatures features = {};

	auto deviceExtensions = std::vector<const char*> {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkDeviceCreateInfo deviceInfo;
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceInfo.pNext = nullptr;
  deviceInfo.flags = 0;
  deviceInfo.queueCreateInfoCount = 1;
  deviceInfo.pQueueCreateInfos = &queueInfo;
  deviceInfo.enabledLayerCount = 0;
  deviceInfo.ppEnabledLayerNames = nullptr;
  deviceInfo.enabledExtensionCount = deviceExtensions.size();
  deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
  deviceInfo.pEnabledFeatures = &features;

	VkDevice device;
	error << vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device);

	return device;
}

auto getFormats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
	std::uint32_t formatCount;
	error << vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
	auto formats = std::vector<VkSurfaceFormatKHR>(formatCount);
	error << vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());

	debug::dump("format", formats);
	return formats;
}

auto getPresentModes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
	std::uint32_t presentModeCount;
	error << vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
	auto presentModes = std::vector<VkPresentModeKHR>(presentModeCount);
	error << vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

	debug::dump("present modes", presentModes);
	return presentModes;
}

auto getSurfaceCapabilities(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	error << vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
	debug::dump(surfaceCapabilities);
	return surfaceCapabilities;
}

auto createSurface(VkInstance instance, GLFWwindow* window, VkPhysicalDevice physicalDevice)
{
	VkSurfaceKHR surface;
	error << glfwCreateWindowSurface(instance, window, nullptr, &surface);

	auto surfaceCapabilities = getSurfaceCapabilities(physicalDevice, surface);
	windowSize.x = surfaceCapabilities.maxImageExtent.width;
	windowSize.y = surfaceCapabilities.maxImageExtent.height;

	VkBool32 surfaceSupport;
	error << vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, 0, surface, &surfaceSupport);
	assert(surfaceSupport == VK_TRUE);

	return surface;
}

auto createSwapchain(VkDevice device, VkSurfaceKHR surface)
{
	VkSwapchainCreateInfoKHR swapchainInfo;
	swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchainInfo.pNext = nullptr;
  swapchainInfo.flags = 0;
  swapchainInfo.surface = surface;
  swapchainInfo.minImageCount = 2; // TODO: dependent
  swapchainInfo.imageFormat = image_format; // TODO: dependent
  swapchainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; // TODO: dependent
  swapchainInfo.imageExtent = VkExtent2D{windowSize.x, windowSize.y}; // TODO: dependent
  swapchainInfo.imageArrayLayers = 1;
  swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  swapchainInfo.queueFamilyIndexCount = 0;
  swapchainInfo.pQueueFamilyIndices = nullptr;
  swapchainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapchainInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR; // TODO: dependent
  swapchainInfo.clipped = VK_TRUE;
  swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

	VkSwapchainKHR swapchain;
	error << vkCreateSwapchainKHR(device, &swapchainInfo, nullptr, &swapchain);
	return swapchain;
}

auto getImages(VkDevice device, VkSwapchainKHR swapchain)
{
	std::uint32_t imageCount;
	error << vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
	auto images = std::vector<VkImage>(imageCount);
	error << vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data());
	return images;
}

auto createSwapchainImageViews(VkDevice device, VkSwapchainKHR swapchain)
{
	auto images = getImages(device, swapchain);

	auto imageViews = std::vector<VkImageView>(images.size());
	for (std::size_t i = 0; i < images.size(); ++i) {
		VkImageViewCreateInfo imageViewInfo;
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.pNext = nullptr;
		imageViewInfo.flags = 0;
		imageViewInfo.image = images[i];
		imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format = image_format; 
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = 1;

		error << vkCreateImageView(device, &imageViewInfo, nullptr, &imageViews[i]);
	}
	std::cout << imageViews.size() << " image viws created" << std::endl;
	return imageViews;
}

auto createRenderPass(VkDevice device)
{
	VkAttachmentDescription attachmentDescription;
	attachmentDescription.flags = 0;
	attachmentDescription.format = image_format;
	attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference attachmentReference;
	attachmentReference.attachment = 0;
	attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription;
	subpassDescription.flags = 0;
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.inputAttachmentCount = 0;
	subpassDescription.pInputAttachments = nullptr;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &attachmentReference;
	subpassDescription.pResolveAttachments = nullptr;
	subpassDescription.pDepthStencilAttachment = nullptr;
	subpassDescription.preserveAttachmentCount = 0;
	subpassDescription.pPreserveAttachments = nullptr;

	VkSubpassDependency subpassDependency;
	subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  subpassDependency.dstSubpass = 0;
  subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  subpassDependency.srcAccessMask = 0;
  subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  subpassDependency.dependencyFlags = 0;

	VkRenderPassCreateInfo renderPassInfo;
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.pNext = nullptr;
  renderPassInfo.flags = 0;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &attachmentDescription;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpassDescription;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &subpassDependency;

	VkRenderPass renderPass;
	error << vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass);
	return renderPass;
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

auto createFrameBuffers(VkDevice device, const std::vector<VkImageView>& images, VkRenderPass renderPass)
{
	auto framebuffers = std::vector<VkFramebuffer>(images.size());
	for (std::size_t i = 0; i < images.size(); ++i) {
		VkFramebufferCreateInfo framebufferInfo;
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.pNext = nullptr;
    framebufferInfo.flags = 0;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &images[i];
    framebufferInfo.width = windowSize.x;
    framebufferInfo.height = windowSize.y;
    framebufferInfo.layers = 1;

		error << vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffers[i]);
	}
	return framebuffers;
}

auto createCommandPool(VkDevice device)
{
	VkCommandPoolCreateInfo commandPoolInfo;
	commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  commandPoolInfo.pNext = nullptr;
  // commandPoolInfo.flags = 0;
  commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  commandPoolInfo.queueFamilyIndex = 0; // TODO: dependent

	VkCommandPool commandPool;
	error << vkCreateCommandPool(device, &commandPoolInfo, nullptr, &commandPool);
	return commandPool;
}

auto createCommandBuffers(VkDevice device, VkCommandPool commandPool, std::size_t count)
{
	VkCommandBufferAllocateInfo commandBufferInfo;
	commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferInfo.pNext = nullptr;
  commandBufferInfo.commandPool = commandPool;
  commandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  commandBufferInfo.commandBufferCount = count;

	auto commandBuffers = std::vector<VkCommandBuffer>(count);
	error << vkAllocateCommandBuffers(device, &commandBufferInfo, commandBuffers.data());
	return commandBuffers;
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
	auto instance = createInstance();
	auto window = createWindow();
	auto physicalDevice = getPhysicalDevice(instance);
	auto device = createDevice(physicalDevice);
	auto surface = createSurface(instance, window, physicalDevice);

	auto swapchain = createSwapchain(device, surface);
	auto imageViews = createSwapchainImageViews(device, swapchain);
	auto renderPass = createRenderPass(device);
	auto frameBuffers = createFrameBuffers(device, imageViews, renderPass);
	auto commandPool = createCommandPool(device);
	auto commandBuffers = createCommandBuffers(device, commandPool, imageViews.size());

	auto pipeline = Pipeline(physicalDevice, device, renderPass, imageViews.size());
	// // pipeline descriptors
	// auto [uniformMemories, uniformBuffers] = createUniformBuffers(physicalDevice, device, imageViews.size());
	// auto descriptorLayout = createDescriptorSetLayout(device);
	// auto descriptorLayouts = std::vector<VkDescriptorSetLayout>(imageViews.size(), descriptorLayout);
	// // pipeline
	// auto [shaderVert, shaderFrag] = createShaders(device);
	// auto pipelineLayout = createPipelineLayout(device, descriptorLayouts);
	// auto graphicsPipeline = createPipeline(device, renderPass, pipelineLayout, shaderVert, shaderFrag);

	auto queue = getQueue(device);

	// textures
	auto [textureMemory, textureImage] = createTexture(device, physicalDevice, commandPool, queue, createImage(8));
	auto textureViews = createImageViews(device, textureImage, imageViews.size());
	auto textureSampler = createImageSampler(device);
	auto [textureMemory2, textureImage2] = createTexture(device, physicalDevice, commandPool, queue, createImage(32));
	auto textureViews2 = createImageViews(device, textureImage2, imageViews.size());
	auto textureSampler2 = createImageSampler(device);

	auto descriptorPool = createDescriptorPool(device, imageViews.size());
	auto descriptorSets = createDescriptorSets(device, pipeline.descriptorLayouts, descriptorPool, imageViews.size());
	// actually bind uniform and sampler to an index
	updateDescriptors(device, pipeline.uniformBuffers, textureViews, textureSampler, textureViews2, textureSampler2, descriptorSets, imageViews.size());

	// auto [vertexBufferMemory, vertexBuffer, verticesCount] = createVertexBuffer(device, physicalDevice, commandPool, queue);
	auto [semaphoreImageAvailable, semaphoreRenderingDone] = createSemaphores(device);

	auto renderer = PolygonRenderer();

	// we initialzed vulkan (yay)
	std::cout << "hello vulkan" << std::endl;

	while (!glfwWindowShouldClose(window)) {
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(cloc::now() - startTime) - sleepTime;

		renderer.drawCircle(glm::vec2(0), 1.f, glm::vec3(1));
		renderer.drawTriangle(
			glm::vec2(-1, -1), glm::vec3(1), glm::vec2(0, 0),
			glm::vec2(1, -1), glm::vec3(1), glm::vec2(1, 0),
			glm::vec2(1, 1), glm::vec3(1), glm::vec2(1, 1));
		auto vertices = renderer.flush();
		assert(vertices.size() > 0);
		auto [vertexBufferMemory, vertexBuffer, verticesCount] = createVertexBuffer(device, physicalDevice, commandPool, queue, vertices);
		fillCommandBuffers(commandBuffers, frameBuffers, renderPass, pipeline.pipeline, pipeline.layout, {vertexBuffer}, descriptorSets, verticesCount);
		updateUniformBuffers(device, pipeline.uniformMemories, elapsedTime);
		render(device, swapchain, queue, commandBuffers, semaphoreImageAvailable, semaphoreRenderingDone);
		vkDeviceWaitIdle(device);
		vkFreeMemory(device, vertexBufferMemory, nullptr);
		vkDestroyBuffer(device, vertexBuffer, nullptr);

		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_Q))
			glfwSetWindowShouldClose(window, true);
		while(glfwGetKey(window, GLFW_KEY_W)) {
			auto duration = 10ms;
			sleepTime += duration;
			std::this_thread::sleep_for(duration);
			glfwPollEvents();
		}
		std::this_thread::sleep_for(10ms);
	}

	// destroy vulkan
	vkDeviceWaitIdle(device);

	vkDestroySampler(device, textureSampler, nullptr);
	for (auto textureView : textureViews)
		vkDestroyImageView(device, textureView, nullptr);
	vkDestroyImage(device, textureImage, nullptr);
	vkFreeMemory(device, textureMemory, nullptr);

	vkDestroySampler(device, textureSampler2, nullptr);
	for (auto textureView : textureViews2)
		vkDestroyImageView(device, textureView, nullptr);
	vkDestroyImage(device, textureImage2, nullptr);
	vkFreeMemory(device, textureMemory2, nullptr);

	vkDestroySemaphore(device, semaphoreRenderingDone, nullptr);
	vkDestroySemaphore(device, semaphoreImageAvailable, nullptr);

	// vkFreeDescriptorSets(device, descriptorPool, descriptorSets.size(), descriptorSets.data());
	vkDestroyDescriptorPool(device, descriptorPool, nullptr);

	vkFreeCommandBuffers(device, commandPool, imageViews.size(), commandBuffers.data()); // implicit when pool gets destroyed
	vkDestroyCommandPool(device, commandPool, nullptr);
	for (const auto& framebuffer : frameBuffers)
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);
	for (const auto& imageView : imageViews)
		vkDestroyImageView(device, imageView, nullptr);
	vkDestroySwapchainKHR(device, swapchain, nullptr);

	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
