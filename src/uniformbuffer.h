#pragma once

#include "instance.h"
#include "resource.h"
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
		std::swap(buffer, other.buffer);
		std::swap(memory, other.memory);
		return *this;
	}

	// template <typename Type>
	// static UniformBuffer fromType();
	template <typename Struct>
	static UniformBuffer fromStruct(const Instance& instance);

	template <typename T>
	void update(const T& ubo);

	VkDevice device = VK_NULL_HANDLE;
	Resource<VkBuffer> buffer = VK_NULL_HANDLE;
	Resource<VkDeviceMemory> memory = VK_NULL_HANDLE;
	VkDeviceSize size = 0;
};

using UniformBuffers = std::vector<UniformBuffer>;

// template <typename Type>
// UniformBuffer UniformBuffer::fromType() {
// 	
// }

inline auto createUniformBuffer(const Instance& instance, VkDeviceSize size)
{
	assert(size > 0);

	auto [memory, buffer] = createBuffer(instance.physicalDevice, instance.device, size,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	return std::make_tuple(memory, buffer);
}

template <typename Struct>
UniformBuffer UniformBuffer::fromStruct(const Instance& instance)
{
	auto buffer = UniformBuffer();
	buffer.size = sizeof(Struct);
	buffer.device = instance.device;
	auto [memory, bbuffer] = createUniformBuffer(instance, buffer.size);
	buffer.memory = memory;
	buffer.buffer = bbuffer;
	return buffer;
}

template <typename T>
void UniformBuffer::update(const T& ubo)
{
	void* data;
	vkMapMemory(device, memory, 0, sizeof(T), 0, &data);
	std::memcpy(data, &ubo, sizeof(T));
	vkUnmapMemory(device, memory);
}
