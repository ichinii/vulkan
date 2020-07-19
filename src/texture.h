#pragma once

#include "instance.h"
#include <glm/glm.hpp>

struct Image {
	using color = glm::u8vec4;
	std::vector<color> colors;
	glm::uvec2 size;
};

struct Texture {
	Texture() = default;
	Texture(const Texture&) = delete;
	Texture(Texture&& other);
	~Texture();

	Texture& operator= (const Texture&) = delete;
	Texture& operator= (Texture&& other);

	// parameter uv from (-1, -1) to (1, 1)
	using GeneratorF = glm::vec4(glm::vec2 uv);
	static Texture fromGenerator(const Instance& instance, glm::uvec2 size, std::function<GeneratorF> f);
	static Texture fromFile(const Instance& instance, std::filesystem::path filepath);
	static Texture fromImage(const Instance& instance, Image Image);

	// void update();

	VkDevice device;
	VkDeviceMemory memory = VK_NULL_HANDLE;
	VkImage image = VK_NULL_HANDLE;
	VkSampler sampler = VK_NULL_HANDLE;
	std::vector<VkImageView> views;
};

using Textures = std::vector<Texture>;
