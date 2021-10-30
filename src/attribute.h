#pragma once

#include "graphics.h"

struct AttributeInfo {
	int location;
	int binding;
	VkFormat format;
	std::uint32_t offset;
};

using AttributeInfos = std::vector<AttributeInfo>;
