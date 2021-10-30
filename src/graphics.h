#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
// #include <filesystem>
#include <functional>

constexpr const auto window_size = glm::uvec2{1280, 720};
constexpr const auto image_format = VK_FORMAT_B8G8R8A8_UNORM;
static auto windowSize = window_size;

extern int findMemoryType(VkPhysicalDevice physicalDevice, VkMemoryRequirements requirements, VkMemoryPropertyFlags memoryFlags);
extern std::tuple<VkDeviceMemory, VkBuffer> createBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryFlags);
extern VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool);
extern void endSingleTimeCommands(VkDevice device, VkCommandBuffer commandBuffer, VkCommandPool commandPool, VkQueue queue);
