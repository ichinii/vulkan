#include "graphics.h"
#include "log.h"

int findMemoryType(VkPhysicalDevice physicalDevice, VkMemoryRequirements requirements, VkMemoryPropertyFlags memoryFlags)
{
	int memoryIndex = -1;
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
	for (std::size_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
		auto types = memoryProperties.memoryTypes[i];
		auto enabled = 0 != (requirements.memoryTypeBits & (1 << i));
		auto matching = ((memoryFlags & types.propertyFlags) == memoryFlags);
		// std::cout << "\tmemory properties" << std::endl
		// 	<< "\t\theap index: " << types.heapIndex << std::endl
		// 	<< "\t\tenabled: " << enabled << std::endl
		// 	<< "\t\tmatching requirements: " << matching << std::endl;
		// debug::dump(memoryProperties.memoryTypes[i].propertyFlags);
		
		if (memoryIndex == -1 && enabled && matching)
			memoryIndex = i;
	}
	assert(memoryIndex != -1);
	return memoryIndex;
}

std::tuple<VkDeviceMemory, VkBuffer> createBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryFlags)
{
	VkBufferCreateInfo bufferInfo;
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.pNext = nullptr;
  bufferInfo.flags = 0;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  bufferInfo.queueFamilyIndexCount = 0;
  bufferInfo.pQueueFamilyIndices = nullptr;

	VkBuffer buffer;
	error << vkCreateBuffer(device, &bufferInfo, nullptr, &buffer);

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(device, buffer, &requirements);
	// debug::dump(requirements);

	int memoryIndex = findMemoryType(physicalDevice, requirements, memoryFlags);

	VkMemoryAllocateInfo memoryInfo;
	memoryInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  memoryInfo.pNext = nullptr;
	memoryInfo.allocationSize = requirements.size; 
  memoryInfo.memoryTypeIndex = memoryIndex;

	VkDeviceMemory memory;
	error << vkAllocateMemory(device, &memoryInfo, nullptr, &memory);
	
	vkBindBufferMemory(device, buffer, memory, 0);

	return std::make_tuple(memory, buffer);
}
