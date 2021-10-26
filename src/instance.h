#pragma once

#include "graphics.h"
#include "resource.h"

class Instance {
public:
	Instance();
	Instance(const Instance&) = delete;
	Instance(Instance&&) = default;
	~Instance();

	Instance& operator= (const Instance&) = delete;
	Instance& operator= (Instance&&) = default;

	GLFWwindow* window;
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkSurfaceKHR surface;

	VkSwapchainKHR swapchain;
	std::vector<VkImageView> imageViews;
	Resource<VkCommandPool> commandPool;
	Resource<VkCommandBuffer> commandBuffer;

	VkQueue queue;
};

