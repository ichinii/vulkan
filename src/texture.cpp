#include "texture.h"

Texture Texture::fromGenerator(const Instance& instance, glm::uvec2 size, std::function<Texture::GeneratorF> f) {
	auto colors = std::vector<ImageColor>(size.x * size.y);
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

Texture Texture::fromFile(const Instance& instance, const char* filepath) {
	assert(false), (void)"unimplemented";
	return {};
}

Texture Texture::fromImage(const Instance& instance, Image image) {
	Texture texture;
	auto [memory, textureImage] = createTexture(instance.device, instance.physicalDevice, instance.commandPool, instance.queue, image);
	auto view = createColorImageView(instance.device, textureImage);
	auto sampler = createImageSampler(instance.device);
	texture.device = instance.device;
	texture.memory = memory;
	texture.image = textureImage;
	texture.view = view;
	texture.sampler = sampler;
	return texture;
}

Texture::~Texture()
{
	vkDestroySampler(device, sampler, nullptr);
	vkDestroyImageView(device, view, nullptr);
	vkDestroyImage(device, image, nullptr);
	vkFreeMemory(device, memory, nullptr);
}
