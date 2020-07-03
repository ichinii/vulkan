#include "instance.h"
#include "log.h"

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

auto getQueue(VkDevice device)
{
	VkQueue queue;
	vkGetDeviceQueue(device, 0, 0, &queue);
	return queue;
}

Instance::Instance()
{
	window = createWindow();
	instance = createInstance();
	physicalDevice = getPhysicalDevice(instance);
	device = createDevice(physicalDevice);
	surface = createSurface(instance, window, physicalDevice);

	swapchain = createSwapchain(device, surface);
	imageViews = createSwapchainImageViews(device, swapchain);
	renderPass = createRenderPass(device);
	frameBuffers = createFrameBuffers(device, imageViews, renderPass);
	commandPool = createCommandPool(device);
	commandBuffers = createCommandBuffers(device, commandPool, imageViews.size());

	queue = getQueue(device);
}

Instance::~Instance()
{
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
}
