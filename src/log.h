#pragma once

#include <unordered_map>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <functional>
#include "graphics.h"

namespace debug {

extern std::unordered_map<std::underlying_type_t<VkResult>, std::string> error_string;
extern std::unordered_map<std::underlying_type_t<VkPhysicalDeviceType>, std::string> physical_device_type_name;
extern std::unordered_map<std::underlying_type_t<VkFormat>, std::string> format_string;
extern std::unordered_map<std::underlying_type_t<VkPresentModeKHR>, std::string> present_mode_string;
extern std::unordered_map<std::underlying_type_t<VkColorSpaceKHR>, std::string> color_space_string;
extern std::unordered_map<std::underlying_type_t<VkColorSpaceKHR>, std::string> color_space_string;

extern void dump(const VkPhysicalDeviceLimits& limits);
extern void dump(const VkPhysicalDeviceFeatures& features);
extern void dump(const VkPhysicalDevice& device);
extern void dump(const VkLayerProperties &layer);
extern void dump(const VkExtensionProperties &extension);
extern void dump(const VkSurfaceCapabilitiesKHR& capabilities);
extern void dump(const VkSurfaceFormatKHR& format);
extern void dump(const VkPresentModeKHR& mode);
extern void dump(const VkMemoryRequirements& requirements);
extern void dump(const VkMemoryPropertyFlags& bits);

template <typename T>
inline void dump(std::string name, const std::vector<T>& container)
{
	std::cout << name << " count: " << container.size() << std::endl;
	std::cout << name << std::endl;
	for (const auto& element : container)
		dump(element);
}

template <typename T>
inline void dump_direct(const std::string& name, const std::vector<T>& container)
{
	std::cout << name << std::endl;
	for (const auto& element : container) {
		std::cout << '\t' << element << std::endl;
	}
}

} // namespace debug

struct ErrorObject {
	const char* file;
	int line;

	void operator<< (VkResult error_code) {
		if (error_code != VK_SUCCESS) {
			std::cout << "fatal: vukan error (" << file << ": " << line << "): "
				<< error_code << " (" << debug::error_string[error_code] << ")" << std::endl;
			std::abort();
		}
	}
};

#define error ErrorObject {__FILE__, __LINE__}

template <typename T, typename A>
inline auto& operator << (std::ostream& os, const std::vector<T, A>& v) {
	auto first = true;
	os << "{";
	for (const auto& i : v) {
		if (!first)
			os << ", ";
		os << i;
	}
	return os << "}";
}

template <int D, typename T, glm::qualifier Q>
inline auto& operator << (std::ostream& os, glm::vec<D, T, Q> v) {
	os << "(";
	for (auto i = 0; i < D - 1; ++i)
		os << v[i] << ", ";
	if (D > 0)
		os << v[D - 1];
	return os << ")";
}
