#include <cassert>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include "graphics.h"
#include "shader.h"
#include "log.h"
#include <glm/glm.hpp>

constexpr auto initialWindowSize = glm::uvec2{1280, 720};
constexpr auto initialImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

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
	auto extensionProperties = getExtensionProperties;

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
	GLFWwindow* window = glfwCreateWindow(initialWindowSize.x, initialWindowSize.y, "Vulkan", nullptr, nullptr);
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

auto createDevice(VkInstance instance)
{
	auto physicalDevice = getPhysicalDevice(instance);

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

auto createSurface(VkInstance instance, GLFWwindow* window, VkPhysicalDevice physicalDevice)
{
	VkSurfaceKHR surface;
	error << glfwCreateWindowSurface(instance, window, nullptr, &surface);
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	error << vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

	VkBool32 surfaceSupport;
	error << vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, 0, surface, &surfaceSupport);
	assert(surfaceSupport == VK_TRUE);

	debug::dump(surfaceCapabilities);
	return surface;
}

auto createSwapChain(VkDevice device, VkSurfaceKHR surface)
{
	VkSwapchainCreateInfoKHR swapchainInfo;
	swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchainInfo.pNext = nullptr;
  swapchainInfo.flags = 0;
  swapchainInfo.surface = surface;
  swapchainInfo.minImageCount = 2; // TODO: dependent
  swapchainInfo.imageFormat = initialImageFormat; // TODO: dependent
  swapchainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; // TODO: dependent
  swapchainInfo.imageExtent = VkExtent2D{initialWindowSize.x, initialWindowSize.y}; // TODO: dependent
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

auto createImageViews(VkDevice device, VkSwapchainKHR swapchain)
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
		imageViewInfo.format = initialImageFormat; 
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
	attachmentDescription.format = initialImageFormat;
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

auto createPipelineLayout(VkDevice device)
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.pNext = nullptr;
  pipelineLayoutInfo.flags = 0;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;

	VkPipelineLayout pipelineLayout;
	error << vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout);
	return pipelineLayout;
}

auto createShaders(VkDevice device)
{
	auto shaderCodeVert = readShaderFile("res/shader.vert.spv");
	auto shaderCodeFrag = readShaderFile("res/shader.frag.spv");

	auto shaderVert = createShader(device, shaderCodeVert);
	auto shaderFrag = createShader(device, shaderCodeFrag);

	return std::make_tuple(shaderVert, shaderFrag);
}

auto createPipeline(VkDevice device, VkRenderPass renderPass, VkPipelineLayout pipelineLayout, VkShaderModule shaderVert, VkShaderModule shaderFrag)
{

	VkPipelineShaderStageCreateInfo shaderStageInfoVert;
	shaderStageInfoVert.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfoVert.pNext = nullptr;
	shaderStageInfoVert.flags = 0;
	shaderStageInfoVert.stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStageInfoVert.module = shaderVert;
	shaderStageInfoVert.pName = "main";
	shaderStageInfoVert.pSpecializationInfo = nullptr;

	VkPipelineShaderStageCreateInfo shaderStageInfoFrag;
	shaderStageInfoFrag.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfoFrag.pNext = nullptr;
	shaderStageInfoFrag.flags = 0;
	shaderStageInfoFrag.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStageInfoFrag.module = shaderFrag;
	shaderStageInfoFrag.pName = "main";
	shaderStageInfoFrag.pSpecializationInfo = nullptr;

	auto shaderStageInfos = std::vector<VkPipelineShaderStageCreateInfo> {shaderStageInfoVert, shaderStageInfoFrag};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.pNext = nullptr;
	vertexInputInfo.flags = 0;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexBindingDescriptions = nullptr;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr;

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.pNext = nullptr;
	inputAssemblyInfo.flags = 0;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = initialWindowSize.x;
	viewport.height = initialWindowSize.y;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;

	VkRect2D scissor;
	scissor.offset = {0, 0};
	scissor.extent = {initialWindowSize.x, initialWindowSize.y};

	VkPipelineViewportStateCreateInfo viewportStateInfo;
	viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateInfo.pNext = nullptr;
	viewportStateInfo.flags = 0;
	viewportStateInfo.viewportCount = 1;
	viewportStateInfo.pViewports = &viewport;
	viewportStateInfo.scissorCount = 1;
	viewportStateInfo.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizationInfo.pNext = nullptr;
  rasterizationInfo.flags = 0;
  rasterizationInfo.depthClampEnable = VK_FALSE;
  rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
  rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizationInfo.depthBiasEnable = VK_FALSE;
  rasterizationInfo.depthBiasConstantFactor = 0.f;
  rasterizationInfo.depthBiasClamp = 0.f;
  rasterizationInfo.depthBiasSlopeFactor = 0.f;
  rasterizationInfo.lineWidth = 1.f;

	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampleInfo.pNext = nullptr;
  multisampleInfo.flags = 0;
  multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampleInfo.sampleShadingEnable = VK_FALSE;
  multisampleInfo.minSampleShading = 1.f;
  multisampleInfo.pSampleMask = nullptr;
  multisampleInfo.alphaToCoverageEnable = VK_FALSE;
  multisampleInfo.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendInfo.pNext = nullptr;
	colorBlendInfo.flags = 0;
	colorBlendInfo.logicOpEnable = VK_FALSE;
	colorBlendInfo.logicOp = VK_LOGIC_OP_NO_OP;
	colorBlendInfo.attachmentCount = 1;
	colorBlendInfo.pAttachments = &colorBlendAttachment;
	colorBlendInfo.blendConstants[0] = 0.f;
	colorBlendInfo.blendConstants[1] = 0.f;
	colorBlendInfo.blendConstants[2] = 0.f;
	colorBlendInfo.blendConstants[3] = 0.f;

	VkGraphicsPipelineCreateInfo graphicsPipelineInfo;
	graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  graphicsPipelineInfo.pNext = nullptr;
  graphicsPipelineInfo.flags = 0;
  graphicsPipelineInfo.stageCount = 2;
  graphicsPipelineInfo.pStages = shaderStageInfos.data();
  graphicsPipelineInfo.pVertexInputState = &vertexInputInfo;
  graphicsPipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
  graphicsPipelineInfo.pTessellationState = nullptr;
  graphicsPipelineInfo.pViewportState = &viewportStateInfo;
  graphicsPipelineInfo.pRasterizationState = &rasterizationInfo;
  graphicsPipelineInfo.pMultisampleState = &multisampleInfo;
  graphicsPipelineInfo.pDepthStencilState = nullptr;
  graphicsPipelineInfo.pColorBlendState = &colorBlendInfo;
  graphicsPipelineInfo.pDynamicState = nullptr;
  graphicsPipelineInfo.layout = pipelineLayout;
  graphicsPipelineInfo.renderPass = renderPass;
  graphicsPipelineInfo.subpass = 0;
  graphicsPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  graphicsPipelineInfo.basePipelineIndex = -1;

	VkPipeline graphicsPipeline;
	error << vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &graphicsPipelineInfo, nullptr, &graphicsPipeline);

	return graphicsPipeline;
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
    framebufferInfo.width = initialWindowSize.x;
    framebufferInfo.height = initialWindowSize.y;
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
  commandPoolInfo.flags = 0;
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

auto fillCommandBuffers(const std::vector<VkCommandBuffer>& commandBuffers, const std::vector<VkFramebuffer>& framebuffers, VkRenderPass renderPass, VkPipeline graphicsPipeline)
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
    renderPassBeginInfo.renderArea.extent = {initialWindowSize.x, initialWindowSize.y};

		VkClearValue clearValue = {0.f, 0.f, 0.f, 1.f};
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &clearValue;

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
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

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	std::cout << std::boolalpha;

	initGlfw();
	auto instance = createInstance();
	auto window = createWindow();
	auto device = createDevice(instance);
	auto physicalDevice = getPhysicalDevice(instance);
	auto surface = createSurface(instance, window, physicalDevice);
	auto swapchain = createSwapChain(device, surface);
	auto imageViews = createImageViews(device, swapchain);
	auto renderPass = createRenderPass(device);
	auto [shaderVert, shaderFrag] = createShaders(device);
	auto pipelineLayout = createPipelineLayout(device);
	auto graphicsPipeline = createPipeline(device, renderPass, pipelineLayout, shaderVert, shaderFrag);
	auto frameBuffers = createFrameBuffers(device, imageViews, renderPass);
	auto commandPool = createCommandPool(device);
	auto commandBuffers = createCommandBuffers(device, commandPool, imageViews.size());

	// we initialzed vulkan (yay)
	std::cout << "hello vulkan" << std::endl;

	fillCommandBuffers(commandBuffers, frameBuffers, renderPass, graphicsPipeline);
	auto queue = getQueue(device);
	auto [semaphoreImageAvailable, semaphoreRenderingDone] = createSemaphores(device);

	while (!glfwWindowShouldClose(window)) {
		render(device, swapchain, queue, commandBuffers, semaphoreImageAvailable, semaphoreRenderingDone);

		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_Q))
			glfwSetWindowShouldClose(window, true);
	}

	// destroy vulkan
	vkDeviceWaitIdle(device);
	vkDestroySemaphore(device, semaphoreRenderingDone, nullptr);
	vkDestroySemaphore(device, semaphoreImageAvailable, nullptr);
	vkFreeCommandBuffers(device, commandPool, imageViews.size(), commandBuffers.data()); // implicit when pool gets destroyed
	vkDestroyCommandPool(device, commandPool, nullptr);
	for (const auto& framebuffer : frameBuffers)
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyShaderModule(device, shaderVert, nullptr);
	vkDestroyShaderModule(device, shaderFrag, nullptr);
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
