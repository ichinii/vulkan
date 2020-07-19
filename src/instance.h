#pragma once

#include "graphics.h"

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
	VkRenderPass renderPass;
	std::vector<VkFramebuffer> frameBuffers;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	VkQueue queue;
};

