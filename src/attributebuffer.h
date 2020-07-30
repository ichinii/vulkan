#pragma once

#include "instance.h"

class AttributeBuffer {
public:
	AttributeBuffer() = default;
	AttributeBuffer(const AttributeBuffer&) = delete;
	AttributeBuffer(AttributeBuffer&&) = default;
	~AttributeBuffer();

	AttributeBuffer& operator= (const AttributeBuffer&) = delete;
	AttributeBuffer& operator= (AttributeBuffer&& other) {
		device = other.device;
		size = other.size;
		std::swap(buffer, other.buffer);
		std::swap(memory, other.memory);
		return *this;
	}

	VkDevice device;
	VkDeviceMemory memory;
	VkBuffer buffer;
	std::size_t size;
};
