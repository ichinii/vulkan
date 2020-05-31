#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#include <functional>
#include "graphics.h"

#define concat2(a, b) a ## b
#define concat(a, b) concat2(a, b)
#define unique_name concat(unique_name_, __LINE__)
#define error                                                                      \
struct {                                                                           \
	void operator<< (VkResult error_code) {                                          \
		if (error_code != VK_SUCCESS) {                                                \
			std::cout << "fatal: vukan error (" << __FILE__ << ": " << __LINE__ << "): " \
				<< error_code << " ("<< log::error_string[error_code] << ")" << std::endl; \
			std::abort();                                                                \
		}                                                                              \
	}                                                                                \
} unique_name; unique_name

namespace log {
extern std::unordered_map<std::underlying_type_t<VkResult>, std::string> error_string;
extern std::unordered_map<std::underlying_type_t<VkPhysicalDeviceType>, std::string> physical_device_type_name;
extern std::unordered_map<std::underlying_type_t<VkFormat>, std::string> format_string;
extern std::unordered_map<std::underlying_type_t<VkPresentModeKHR>, std::string> present_mode_string;
extern std::unordered_map<std::underlying_type_t<VkColorSpaceKHR>, std::string> color_space_string;

extern void dump(const VkPhysicalDeviceLimits& limits);
extern void dump(const VkPhysicalDeviceFeatures& features);
extern void dump(const VkPhysicalDevice& device);
extern void dump(const VkLayerProperties &layer);
extern void dump(const VkExtensionProperties &extension);
extern void dump(const VkSurfaceCapabilitiesKHR& capabilities);
extern void dump(const VkSurfaceFormatKHR& format);
extern void dump(const VkPresentModeKHR& mode);

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

};
