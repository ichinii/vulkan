#pragma once

#include <glm/glm.hpp>
#include "instance.h"

struct Image {
	using color = glm::u8vec4;
	std::vector<color> colors;
	glm::uvec2 size;
};

struct Texture {
	// parameter uv from (-1, -1) to (1, 1)
	using GeneratorF = glm::vec4(glm::vec2 uv);
	static Texture fromGenerator(const Instance& instance, glm::uvec2 size, std::function<GeneratorF> f);
	static Texture fromFile(const Instance& instance, std::filesystem::path filepath);
	static Texture fromImage(const Instance& instance, Image Image);
	~Texture();

	VkDevice device;
	VkDeviceMemory memory;
	VkImage image;
	VkSampler sampler;
	std::vector<VkImageView> views;
};
