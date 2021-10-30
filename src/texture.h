#pragma once

#include "instance.h"
#include "resource.h"
#include <glm/glm.hpp>

struct Image {
	using color = glm::u8vec4;
	std::vector<color> colors;
	glm::uvec2 size;
};

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
	Resource<VkImageView> view;
};

using Textures = std::vector<Texture>;
