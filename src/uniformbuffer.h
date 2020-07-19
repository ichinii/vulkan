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
	UniformBuffer& operator= (UniformBuffer&&) = default;

	// template <typename Type>
	// static UniformBuffer fromType();
	template <typename Struct, typename ...Members>
	static UniformBuffer fromStruct(const Instance& instance, Members Struct::* ... m);

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

inline auto createUniformBuffers(const Instance& instance, VkDeviceSize size, std::size_t count)
{
	assert(size > 0);

	auto memories = std::vector<VkDeviceMemory>(size);
	auto buffers = std::vector<VkBuffer>(size);

	for (std::size_t i = 0; i < count; ++i) {
		auto [memory, buffer] = createBuffer(instance.physicalDevice, instance.device, size,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		memories[i] = memory;
		buffers[i] = buffer;
	}

	return std::make_tuple(memories, buffers);
}

template <typename Struct, typename ...Members>
UniformBuffer UniformBuffer::fromStruct(const Instance& instance, Members Struct::* ... m)
{
	auto buffer = UniformBuffer();
	buffer.size = sizeof(Struct);
	buffer.device = instance.device;
	auto [memories, buffers] = createUniformBuffers(instance, buffer.size, instance.imageViews.size());
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
