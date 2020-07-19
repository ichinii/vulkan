#include "uniformbuffer.h"

UniformBuffer::~UniformBuffer()
{
	for (const auto& b : buffers)
		vkDestroyBuffer(device, b, nullptr);
	for (const auto& m : memories)
		vkFreeMemory(device, m, nullptr);
}
