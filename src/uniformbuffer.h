#pragma once

#include "instance.h"
#include <cstring>

class UniformBuffer {
public:
	UniformBuffer() = default;
	UniformBuffer(const UniformBuffer&) = delete;
	UniformBuffer(UniformBuffer&&) = default;
	~UniformBuffer();

	UniformBuffer& operator= (const UniformBuffer&) = delete;
	UniformBuffer& operator= (UniformBuffer&& other) {
		device = other.device;
		size = other.size;
		std::swap(buffers, other.buffers);
		std::swap(memories, other.memories);
		return *this;
	}

	// template <typename Type>
	// static UniformBuffer fromType();
	template <typename Struct>
	static UniformBuffer fromStruct(const Instance& instance);

	template <typename T>
	void update(const T& ubo);

	VkDevice device;
	std::vector<VkBuffer> buffers;
	std::vector<VkDeviceMemory> memories;
	VkDeviceSize size;
};

using UniformBuffers = std::vector<UniformBuffer>;

// template <typename Type>
// UniformBuffer UniformBuffer::fromType() {
// 	
// }

inline auto createUniformBuffer(const Instance& instance, VkDeviceSize size, std::size_t count)
{
	assert(size > 0);

	auto memories = std::vector<VkDeviceMemory>(count);
	auto buffers = std::vector<VkBuffer>(count);

	for (std::size_t i = 0; i < count; ++i) {
		auto [memory, buffer] = createBuffer(instance.physicalDevice, instance.device, size,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		memories[i] = memory;
		buffers[i] = buffer;
	}

	return std::make_tuple(memories, buffers);
}

template <typename Struct>
UniformBuffer UniformBuffer::fromStruct(const Instance& instance)
{
	auto buffer = UniformBuffer();
	buffer.size = sizeof(Struct);
	buffer.device = instance.device;
	auto [memories, buffers] = createUniformBuffer(instance, buffer.size, instance.imageViews.size());
	buffer.memories = memories;
	buffer.buffers = buffers;
	return buffer;
}

template <typename T>
void UniformBuffer::update(const T& ubo)
{
	for (auto memory : memories) {
		void* data;
		vkMapMemory(device, memory, 0, sizeof(T), 0, &data);
		std::memcpy(data, &ubo, sizeof(T));
		vkUnmapMemory(device, memory);
	}
}
