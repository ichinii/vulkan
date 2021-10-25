#include "uniformbuffer.h"

#include <iostream>
UniformBuffer::~UniformBuffer()
{
	vkDestroyBuffer(device, buffer, nullptr);
	vkFreeMemory(device, memory, nullptr);
}
