#pragma once

#include "log.h"

using ShaderCodeChar = std::uint32_t;
using ShaderCode = std::vector<ShaderCodeChar>;

inline ShaderCode readShaderFile(const char* filepath)
{
	std::ifstream file(filepath, std::ios::binary | std::ios::ate);
	assert(file.is_open());
	std::size_t size = file.tellg();
	assert(size % sizeof(ShaderCodeChar) == 0);
	file.seekg(0);
	auto content = ShaderCode(size / sizeof(ShaderCodeChar));
	file.read(reinterpret_cast<char*>(content.data()), size);
	return content;
}

inline VkShaderModule createShader(VkDevice device, const char* filepath)
{
	auto code = readShaderFile(filepath);
	VkShaderModuleCreateInfo shaderInfo;
	shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderInfo.pNext = nullptr;
	shaderInfo.flags = 0;
	shaderInfo.codeSize = code.size() * sizeof(ShaderCode::value_type);
	shaderInfo.pCode = code.data();

	VkShaderModule shader;
	error << vkCreateShaderModule(device, &shaderInfo, nullptr, &shader);
	return shader;
}
