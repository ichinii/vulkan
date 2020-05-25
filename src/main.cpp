#include <bits/stdc++.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

using namespace std::string_literals;
using namespace std::chrono_literals;

std::unordered_map<std::underlying_type_t<VkResult>, std::string> error_string {
	{VK_SUCCESS,                                            "VK_SUCCESS"},
	{VK_NOT_READY,                                          "VK_NOT_READY"},
	{VK_TIMEOUT,                                            "VK_TIMEOUT"},
	{VK_EVENT_SET,                                          "VK_EVENT_SET"},
	{VK_EVENT_RESET,                                        "VK_EVENT_RESET"},
	{VK_INCOMPLETE,                                         "VK_INCOMPLETE"},
	{VK_ERROR_OUT_OF_HOST_MEMORY,                           "VK_ERROR_OUT_OF_HOST_MEMORY"},
	{VK_ERROR_OUT_OF_DEVICE_MEMORY,                         "VK_ERROR_OUT_OF_DEVICE_MEMORY"},
	{VK_ERROR_INITIALIZATION_FAILED,                        "VK_ERROR_INITIALIZATION_FAILED"},
	{VK_ERROR_DEVICE_LOST,                                  "VK_ERROR_DEVICE_LOST"},
	{VK_ERROR_MEMORY_MAP_FAILED,                            "VK_ERROR_MEMORY_MAP_FAILED"},
	{VK_ERROR_LAYER_NOT_PRESENT,                            "VK_ERROR_LAYER_NOT_PRESENT"},
	{VK_ERROR_EXTENSION_NOT_PRESENT,                        "VK_ERROR_EXTENSION_NOT_PRESENT"},
	{VK_ERROR_FEATURE_NOT_PRESENT,                          "VK_ERROR_FEATURE_NOT_PRESENT"},
	{VK_ERROR_INCOMPATIBLE_DRIVER,                          "VK_ERROR_INCOMPATIBLE_DRIVER"},
	{VK_ERROR_TOO_MANY_OBJECTS,                             "VK_ERROR_TOO_MANY_OBJECTS"},
	{VK_ERROR_FORMAT_NOT_SUPPORTED,                         "VK_ERROR_FORMAT_NOT_SUPPORTED"},
	{VK_ERROR_FRAGMENTED_POOL,                              "VK_ERROR_FRAGMENTED_POOL"},
	{VK_ERROR_UNKNOWN,                                      "VK_ERROR_UNKNOWN"},
	{VK_ERROR_OUT_OF_POOL_MEMORY,                           "VK_ERROR_OUT_OF_POOL_MEMORY"},
	{VK_ERROR_INVALID_EXTERNAL_HANDLE,                      "VK_ERROR_INVALID_EXTERNAL_HANDLE"},
	{VK_ERROR_FRAGMENTATION,                                "VK_ERROR_FRAGMENTATION"},
	{VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS,               "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS"},
	{VK_ERROR_SURFACE_LOST_KHR,                             "VK_ERROR_SURFACE_LOST_KHR"},
	{VK_ERROR_NATIVE_WINDOW_IN_USE_KHR,                     "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR"},
	{VK_SUBOPTIMAL_KHR,                                     "VK_SUBOPTIMAL_KHR"},
	{VK_ERROR_OUT_OF_DATE_KHR,                              "VK_ERROR_OUT_OF_DATE_KHR"},
	{VK_ERROR_INCOMPATIBLE_DISPLAY_KHR,                     "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR"},
	{VK_ERROR_VALIDATION_FAILED_EXT,                        "VK_ERROR_VALIDATION_FAILED_EXT"},
	{VK_ERROR_INVALID_SHADER_NV,                            "VK_ERROR_INVALID_SHADER_NV"},
	{VK_ERROR_INCOMPATIBLE_VERSION_KHR,                     "VK_ERROR_INCOMPATIBLE_VERSION_KHR"},
	{VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT, "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT"},
	{VK_ERROR_NOT_PERMITTED_EXT,                            "VK_ERROR_NOT_PERMITTED_EXT"},
	{VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT,          "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT"},
	{VK_THREAD_IDLE_KHR,                                    "VK_THREAD_IDLE_KHR"},
	{VK_THREAD_DONE_KHR,                                    "VK_THREAD_DONE_KHR"},
	{VK_OPERATION_DEFERRED_KHR,                             "VK_OPERATION_DEFERRED_KHR"},
	{VK_OPERATION_NOT_DEFERRED_KHR,                         "VK_OPERATION_NOT_DEFERRED_KHR"},
	{VK_PIPELINE_COMPILE_REQUIRED_EXT,                      "VK_PIPELINE_COMPILE_REQUIRED_EXT"},
	{VK_ERROR_OUT_OF_POOL_MEMORY_KHR,                       "VK_ERROR_OUT_OF_POOL_MEMORY_KHR"},
	{VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR,                  "VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR"},
	{VK_ERROR_FRAGMENTATION_EXT,                            "VK_ERROR_FRAGMENTATION_EXT"},
	{VK_ERROR_INVALID_DEVICE_ADDRESS_EXT,                   "VK_ERROR_INVALID_DEVICE_ADDRESS_EXT"},
	{VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR,           "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR"},
	{VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT,                "VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT"},
};

std::unordered_map<std::underlying_type_t<VkPhysicalDeviceType>, std::string> physical_device_type_name {
	{VK_PHYSICAL_DEVICE_TYPE_OTHER,          "VK_PHYSICAL_DEVICE_TYPE_OTHER"},
	{VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU, "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU"},
	{VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,   "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU"},
	{VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,    "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU"},
	{VK_PHYSICAL_DEVICE_TYPE_CPU,            "VK_PHYSICAL_DEVICE_TYPE_CPU"},
};

std::unordered_map<std::underlying_type_t<VkFormat>, std::string> format_string {
	{VK_FORMAT_UNDEFINED,                                      "VK_FORMAT_UNDEFINED"},
	{VK_FORMAT_R4G4_UNORM_PACK8,                               "VK_FORMAT_R4G4_UNORM_PACK8"},
	{VK_FORMAT_R4G4B4A4_UNORM_PACK16,                          "VK_FORMAT_R4G4B4A4_UNORM_PACK16"},
	{VK_FORMAT_B4G4R4A4_UNORM_PACK16,                          "VK_FORMAT_B4G4R4A4_UNORM_PACK16"},
	{VK_FORMAT_R5G6B5_UNORM_PACK16,                            "VK_FORMAT_R5G6B5_UNORM_PACK16"},
	{VK_FORMAT_B5G6R5_UNORM_PACK16,                            "VK_FORMAT_B5G6R5_UNORM_PACK16"},
	{VK_FORMAT_R5G5B5A1_UNORM_PACK16,                          "VK_FORMAT_R5G5B5A1_UNORM_PACK16"},
	{VK_FORMAT_B5G5R5A1_UNORM_PACK16,                          "VK_FORMAT_B5G5R5A1_UNORM_PACK16"},
	{VK_FORMAT_A1R5G5B5_UNORM_PACK16,                          "VK_FORMAT_A1R5G5B5_UNORM_PACK16"},
	{VK_FORMAT_R8_UNORM,                                       "VK_FORMAT_R8_UNORM"},
	{VK_FORMAT_R8_SNORM,                                       "VK_FORMAT_R8_SNORM"},
	{VK_FORMAT_R8_USCALED,                                     "VK_FORMAT_R8_USCALED"},
	{VK_FORMAT_R8_SSCALED,                                     "VK_FORMAT_R8_SSCALED"},
	{VK_FORMAT_R8_UINT,                                        "VK_FORMAT_R8_UINT"},
	{VK_FORMAT_R8_SINT,                                        "VK_FORMAT_R8_SINT"},
	{VK_FORMAT_R8_SRGB,                                        "VK_FORMAT_R8_SRGB"},
	{VK_FORMAT_R8G8_UNORM,                                     "VK_FORMAT_R8G8_UNORM"},
	{VK_FORMAT_R8G8_SNORM,                                     "VK_FORMAT_R8G8_SNORM"},
	{VK_FORMAT_R8G8_USCALED,                                   "VK_FORMAT_R8G8_USCALED"},
	{VK_FORMAT_R8G8_SSCALED,                                   "VK_FORMAT_R8G8_SSCALED"},
	{VK_FORMAT_R8G8_UINT,                                      "VK_FORMAT_R8G8_UINT"},
	{VK_FORMAT_R8G8_SINT,                                      "VK_FORMAT_R8G8_SINT"},
	{VK_FORMAT_R8G8_SRGB,                                      "VK_FORMAT_R8G8_SRGB"},
	{VK_FORMAT_R8G8B8_UNORM,                                   "VK_FORMAT_R8G8B8_UNORM"},
	{VK_FORMAT_R8G8B8_SNORM,                                   "VK_FORMAT_R8G8B8_SNORM"},
	{VK_FORMAT_R8G8B8_USCALED,                                 "VK_FORMAT_R8G8B8_USCALED"},
	{VK_FORMAT_R8G8B8_SSCALED,                                 "VK_FORMAT_R8G8B8_SSCALED"},
	{VK_FORMAT_R8G8B8_UINT,                                    "VK_FORMAT_R8G8B8_UINT"},
	{VK_FORMAT_R8G8B8_SINT,                                    "VK_FORMAT_R8G8B8_SINT"},
	{VK_FORMAT_R8G8B8_SRGB,                                    "VK_FORMAT_R8G8B8_SRGB"},
	{VK_FORMAT_B8G8R8_UNORM,                                   "VK_FORMAT_B8G8R8_UNORM"},
	{VK_FORMAT_B8G8R8_SNORM,                                   "VK_FORMAT_B8G8R8_SNORM"},
	{VK_FORMAT_B8G8R8_USCALED,                                 "VK_FORMAT_B8G8R8_USCALED"},
	{VK_FORMAT_B8G8R8_SSCALED,                                 "VK_FORMAT_B8G8R8_SSCALED"},
	{VK_FORMAT_B8G8R8_UINT,                                    "VK_FORMAT_B8G8R8_UINT"},
	{VK_FORMAT_B8G8R8_SINT,                                    "VK_FORMAT_B8G8R8_SINT"},
	{VK_FORMAT_B8G8R8_SRGB,                                    "VK_FORMAT_B8G8R8_SRGB"},
	{VK_FORMAT_R8G8B8A8_UNORM,                                 "VK_FORMAT_R8G8B8A8_UNORM"},
	{VK_FORMAT_R8G8B8A8_SNORM,                                 "VK_FORMAT_R8G8B8A8_SNORM"},
	{VK_FORMAT_R8G8B8A8_USCALED,                               "VK_FORMAT_R8G8B8A8_USCALED"},
	{VK_FORMAT_R8G8B8A8_SSCALED,                               "VK_FORMAT_R8G8B8A8_SSCALED"},
	{VK_FORMAT_R8G8B8A8_UINT,                                  "VK_FORMAT_R8G8B8A8_UINT"},
	{VK_FORMAT_R8G8B8A8_SINT,                                  "VK_FORMAT_R8G8B8A8_SINT"},
	{VK_FORMAT_R8G8B8A8_SRGB,                                  "VK_FORMAT_R8G8B8A8_SRGB"},
	{VK_FORMAT_B8G8R8A8_UNORM,                                 "VK_FORMAT_B8G8R8A8_UNORM"},
	{VK_FORMAT_B8G8R8A8_SNORM,                                 "VK_FORMAT_B8G8R8A8_SNORM"},
	{VK_FORMAT_B8G8R8A8_USCALED,                               "VK_FORMAT_B8G8R8A8_USCALED"},
	{VK_FORMAT_B8G8R8A8_SSCALED,                               "VK_FORMAT_B8G8R8A8_SSCALED"},
	{VK_FORMAT_B8G8R8A8_UINT,                                  "VK_FORMAT_B8G8R8A8_UINT"},
	{VK_FORMAT_B8G8R8A8_SINT,                                  "VK_FORMAT_B8G8R8A8_SINT"},
	{VK_FORMAT_B8G8R8A8_SRGB,                                  "VK_FORMAT_B8G8R8A8_SRGB"},
	{VK_FORMAT_A8B8G8R8_UNORM_PACK32,                          "VK_FORMAT_A8B8G8R8_UNORM_PACK32"},
	{VK_FORMAT_A8B8G8R8_SNORM_PACK32,                          "VK_FORMAT_A8B8G8R8_SNORM_PACK32"},
	{VK_FORMAT_A8B8G8R8_USCALED_PACK32,                        "VK_FORMAT_A8B8G8R8_USCALED_PACK32"},
	{VK_FORMAT_A8B8G8R8_SSCALED_PACK32,                        "VK_FORMAT_A8B8G8R8_SSCALED_PACK32"},
	{VK_FORMAT_A8B8G8R8_UINT_PACK32,                           "VK_FORMAT_A8B8G8R8_UINT_PACK32"},
	{VK_FORMAT_A8B8G8R8_SINT_PACK32,                           "VK_FORMAT_A8B8G8R8_SINT_PACK32"},
	{VK_FORMAT_A8B8G8R8_SRGB_PACK32,                           "VK_FORMAT_A8B8G8R8_SRGB_PACK32"},
	{VK_FORMAT_A2R10G10B10_UNORM_PACK32,                       "VK_FORMAT_A2R10G10B10_UNORM_PACK32"},
	{VK_FORMAT_A2R10G10B10_SNORM_PACK32,                       "VK_FORMAT_A2R10G10B10_SNORM_PACK32"},
	{VK_FORMAT_A2R10G10B10_USCALED_PACK32,                     "VK_FORMAT_A2R10G10B10_USCALED_PACK32"},
	{VK_FORMAT_A2R10G10B10_SSCALED_PACK32,                     "VK_FORMAT_A2R10G10B10_SSCALED_PACK32"},
	{VK_FORMAT_A2R10G10B10_UINT_PACK32,                        "VK_FORMAT_A2R10G10B10_UINT_PACK32"},
	{VK_FORMAT_A2R10G10B10_SINT_PACK32,                        "VK_FORMAT_A2R10G10B10_SINT_PACK32"},
	{VK_FORMAT_A2B10G10R10_UNORM_PACK32,                       "VK_FORMAT_A2B10G10R10_UNORM_PACK32"},
	{VK_FORMAT_A2B10G10R10_SNORM_PACK32,                       "VK_FORMAT_A2B10G10R10_SNORM_PACK32"},
	{VK_FORMAT_A2B10G10R10_USCALED_PACK32,                     "VK_FORMAT_A2B10G10R10_USCALED_PACK32"},
	{VK_FORMAT_A2B10G10R10_SSCALED_PACK32,                     "VK_FORMAT_A2B10G10R10_SSCALED_PACK32"},
	{VK_FORMAT_A2B10G10R10_UINT_PACK32,                        "VK_FORMAT_A2B10G10R10_UINT_PACK32"},
	{VK_FORMAT_A2B10G10R10_SINT_PACK32,                        "VK_FORMAT_A2B10G10R10_SINT_PACK32"},
	{VK_FORMAT_R16_UNORM,                                      "VK_FORMAT_R16_UNORM"},
	{VK_FORMAT_R16_SNORM,                                      "VK_FORMAT_R16_SNORM"},
	{VK_FORMAT_R16_USCALED,                                    "VK_FORMAT_R16_USCALED"},
	{VK_FORMAT_R16_SSCALED,                                    "VK_FORMAT_R16_SSCALED"},
	{VK_FORMAT_R16_UINT,                                       "VK_FORMAT_R16_UINT"},
	{VK_FORMAT_R16_SINT,                                       "VK_FORMAT_R16_SINT"},
	{VK_FORMAT_R16_SFLOAT,                                     "VK_FORMAT_R16_SFLOAT"},
	{VK_FORMAT_R16G16_UNORM,                                   "VK_FORMAT_R16G16_UNORM"},
	{VK_FORMAT_R16G16_SNORM,                                   "VK_FORMAT_R16G16_SNORM"},
	{VK_FORMAT_R16G16_USCALED,                                 "VK_FORMAT_R16G16_USCALED"},
	{VK_FORMAT_R16G16_SSCALED,                                 "VK_FORMAT_R16G16_SSCALED"},
	{VK_FORMAT_R16G16_UINT,                                    "VK_FORMAT_R16G16_UINT"},
	{VK_FORMAT_R16G16_SINT,                                    "VK_FORMAT_R16G16_SINT"},
	{VK_FORMAT_R16G16_SFLOAT,                                  "VK_FORMAT_R16G16_SFLOAT"},
	{VK_FORMAT_R16G16B16_UNORM,                                "VK_FORMAT_R16G16B16_UNORM"},
	{VK_FORMAT_R16G16B16_SNORM,                                "VK_FORMAT_R16G16B16_SNORM"},
	{VK_FORMAT_R16G16B16_USCALED,                              "VK_FORMAT_R16G16B16_USCALED"},
	{VK_FORMAT_R16G16B16_SSCALED,                              "VK_FORMAT_R16G16B16_SSCALED"},
	{VK_FORMAT_R16G16B16_UINT,                                 "VK_FORMAT_R16G16B16_UINT"},
	{VK_FORMAT_R16G16B16_SINT,                                 "VK_FORMAT_R16G16B16_SINT"},
	{VK_FORMAT_R16G16B16_SFLOAT,                               "VK_FORMAT_R16G16B16_SFLOAT"},
	{VK_FORMAT_R16G16B16A16_UNORM,                             "VK_FORMAT_R16G16B16A16_UNORM"},
	{VK_FORMAT_R16G16B16A16_SNORM,                             "VK_FORMAT_R16G16B16A16_SNORM"},
	{VK_FORMAT_R16G16B16A16_USCALED,                           "VK_FORMAT_R16G16B16A16_USCALED"},
	{VK_FORMAT_R16G16B16A16_SSCALED,                           "VK_FORMAT_R16G16B16A16_SSCALED"},
	{VK_FORMAT_R16G16B16A16_UINT,                              "VK_FORMAT_R16G16B16A16_UINT"},
	{VK_FORMAT_R16G16B16A16_SINT,                              "VK_FORMAT_R16G16B16A16_SINT"},
	{VK_FORMAT_R16G16B16A16_SFLOAT,                            "VK_FORMAT_R16G16B16A16_SFLOAT"},
	{VK_FORMAT_R32_UINT,                                       "VK_FORMAT_R32_UINT"},
	{VK_FORMAT_R32_SINT,                                       "VK_FORMAT_R32_SINT"},
	{VK_FORMAT_R32_SFLOAT,                                     "VK_FORMAT_R32_SFLOAT"},
	{VK_FORMAT_R32G32_UINT,                                    "VK_FORMAT_R32G32_UINT"},
	{VK_FORMAT_R32G32_SINT,                                    "VK_FORMAT_R32G32_SINT"},
	{VK_FORMAT_R32G32_SFLOAT,                                  "VK_FORMAT_R32G32_SFLOAT"},
	{VK_FORMAT_R32G32B32_UINT,                                 "VK_FORMAT_R32G32B32_UINT"},
	{VK_FORMAT_R32G32B32_SINT,                                 "VK_FORMAT_R32G32B32_SINT"},
	{VK_FORMAT_R32G32B32_SFLOAT,                               "VK_FORMAT_R32G32B32_SFLOAT"},
	{VK_FORMAT_R32G32B32A32_UINT,                              "VK_FORMAT_R32G32B32A32_UINT"},
	{VK_FORMAT_R32G32B32A32_SINT,                              "VK_FORMAT_R32G32B32A32_SINT"},
	{VK_FORMAT_R32G32B32A32_SFLOAT,                            "VK_FORMAT_R32G32B32A32_SFLOAT"},
	{VK_FORMAT_R64_UINT,                                       "VK_FORMAT_R64_UINT"},
	{VK_FORMAT_R64_SINT,                                       "VK_FORMAT_R64_SINT"},
	{VK_FORMAT_R64_SFLOAT,                                     "VK_FORMAT_R64_SFLOAT"},
	{VK_FORMAT_R64G64_UINT,                                    "VK_FORMAT_R64G64_UINT"},
	{VK_FORMAT_R64G64_SINT,                                    "VK_FORMAT_R64G64_SINT"},
	{VK_FORMAT_R64G64_SFLOAT,                                  "VK_FORMAT_R64G64_SFLOAT"},
	{VK_FORMAT_R64G64B64_UINT,                                 "VK_FORMAT_R64G64B64_UINT"},
	{VK_FORMAT_R64G64B64_SINT,                                 "VK_FORMAT_R64G64B64_SINT"},
	{VK_FORMAT_R64G64B64_SFLOAT,                               "VK_FORMAT_R64G64B64_SFLOAT"},
	{VK_FORMAT_R64G64B64A64_UINT,                              "VK_FORMAT_R64G64B64A64_UINT"},
	{VK_FORMAT_R64G64B64A64_SINT,                              "VK_FORMAT_R64G64B64A64_SINT"},
	{VK_FORMAT_R64G64B64A64_SFLOAT,                            "VK_FORMAT_R64G64B64A64_SFLOAT"},
	{VK_FORMAT_B10G11R11_UFLOAT_PACK32,                        "VK_FORMAT_B10G11R11_UFLOAT_PACK32"},
	{VK_FORMAT_E5B9G9R9_UFLOAT_PACK32,                         "VK_FORMAT_E5B9G9R9_UFLOAT_PACK32"},
	{VK_FORMAT_D16_UNORM,                                      "VK_FORMAT_D16_UNORM"},
	{VK_FORMAT_X8_D24_UNORM_PACK32,                            "VK_FORMAT_X8_D24_UNORM_PACK32"},
	{VK_FORMAT_D32_SFLOAT,                                     "VK_FORMAT_D32_SFLOAT"},
	{VK_FORMAT_S8_UINT,                                        "VK_FORMAT_S8_UINT"},
	{VK_FORMAT_D16_UNORM_S8_UINT,                              "VK_FORMAT_D16_UNORM_S8_UINT"},
	{VK_FORMAT_D24_UNORM_S8_UINT,                              "VK_FORMAT_D24_UNORM_S8_UINT"},
	{VK_FORMAT_D32_SFLOAT_S8_UINT,                             "VK_FORMAT_D32_SFLOAT_S8_UINT"},
	{VK_FORMAT_BC1_RGB_UNORM_BLOCK,                            "VK_FORMAT_BC1_RGB_UNORM_BLOCK"},
	{VK_FORMAT_BC1_RGB_SRGB_BLOCK,                             "VK_FORMAT_BC1_RGB_SRGB_BLOCK"},
	{VK_FORMAT_BC1_RGBA_UNORM_BLOCK,                           "VK_FORMAT_BC1_RGBA_UNORM_BLOCK"},
	{VK_FORMAT_BC1_RGBA_SRGB_BLOCK,                            "VK_FORMAT_BC1_RGBA_SRGB_BLOCK"},
	{VK_FORMAT_BC2_UNORM_BLOCK,                                "VK_FORMAT_BC2_UNORM_BLOCK"},
	{VK_FORMAT_BC2_SRGB_BLOCK,                                 "VK_FORMAT_BC2_SRGB_BLOCK"},
	{VK_FORMAT_BC3_UNORM_BLOCK,                                "VK_FORMAT_BC3_UNORM_BLOCK"},
	{VK_FORMAT_BC3_SRGB_BLOCK,                                 "VK_FORMAT_BC3_SRGB_BLOCK"},
	{VK_FORMAT_BC4_UNORM_BLOCK,                                "VK_FORMAT_BC4_UNORM_BLOCK"},
	{VK_FORMAT_BC4_SNORM_BLOCK,                                "VK_FORMAT_BC4_SNORM_BLOCK"},
	{VK_FORMAT_BC5_UNORM_BLOCK,                                "VK_FORMAT_BC5_UNORM_BLOCK"},
	{VK_FORMAT_BC5_SNORM_BLOCK,                                "VK_FORMAT_BC5_SNORM_BLOCK"},
	{VK_FORMAT_BC6H_UFLOAT_BLOCK,                              "VK_FORMAT_BC6H_UFLOAT_BLOCK"},
	{VK_FORMAT_BC6H_SFLOAT_BLOCK,                              "VK_FORMAT_BC6H_SFLOAT_BLOCK"},
	{VK_FORMAT_BC7_UNORM_BLOCK,                                "VK_FORMAT_BC7_UNORM_BLOCK"},
	{VK_FORMAT_BC7_SRGB_BLOCK,                                 "VK_FORMAT_BC7_SRGB_BLOCK"},
	{VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK,                        "VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK"},
	{VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK,                         "VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK"},
	{VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK,                      "VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK"},
	{VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK,                       "VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK"},
	{VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK,                      "VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK"},
	{VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK,                       "VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK"},
	{VK_FORMAT_EAC_R11_UNORM_BLOCK,                            "VK_FORMAT_EAC_R11_UNORM_BLOCK"},
	{VK_FORMAT_EAC_R11_SNORM_BLOCK,                            "VK_FORMAT_EAC_R11_SNORM_BLOCK"},
	{VK_FORMAT_EAC_R11G11_UNORM_BLOCK,                         "VK_FORMAT_EAC_R11G11_UNORM_BLOCK"},
	{VK_FORMAT_EAC_R11G11_SNORM_BLOCK,                         "VK_FORMAT_EAC_R11G11_SNORM_BLOCK"},
	{VK_FORMAT_ASTC_4x4_UNORM_BLOCK,                           "VK_FORMAT_ASTC_4x4_UNORM_BLOCK"},
	{VK_FORMAT_ASTC_4x4_SRGB_BLOCK,                            "VK_FORMAT_ASTC_4x4_SRGB_BLOCK"},
	{VK_FORMAT_ASTC_5x4_UNORM_BLOCK,                           "VK_FORMAT_ASTC_5x4_UNORM_BLOCK"},
	{VK_FORMAT_ASTC_5x4_SRGB_BLOCK,                            "VK_FORMAT_ASTC_5x4_SRGB_BLOCK"},
	{VK_FORMAT_ASTC_5x5_UNORM_BLOCK,                           "VK_FORMAT_ASTC_5x5_UNORM_BLOCK"},
	{VK_FORMAT_ASTC_5x5_SRGB_BLOCK,                            "VK_FORMAT_ASTC_5x5_SRGB_BLOCK"},
	{VK_FORMAT_ASTC_6x5_UNORM_BLOCK,                           "VK_FORMAT_ASTC_6x5_UNORM_BLOCK"},
	{VK_FORMAT_ASTC_6x5_SRGB_BLOCK,                            "VK_FORMAT_ASTC_6x5_SRGB_BLOCK"},
	{VK_FORMAT_ASTC_6x6_UNORM_BLOCK,                           "VK_FORMAT_ASTC_6x6_UNORM_BLOCK"},
	{VK_FORMAT_ASTC_6x6_SRGB_BLOCK,                            "VK_FORMAT_ASTC_6x6_SRGB_BLOCK"},
	{VK_FORMAT_ASTC_8x5_UNORM_BLOCK,                           "VK_FORMAT_ASTC_8x5_UNORM_BLOCK"},
	{VK_FORMAT_ASTC_8x5_SRGB_BLOCK,                            "VK_FORMAT_ASTC_8x5_SRGB_BLOCK"},
	{VK_FORMAT_ASTC_8x6_UNORM_BLOCK,                           "VK_FORMAT_ASTC_8x6_UNORM_BLOCK"},
	{VK_FORMAT_ASTC_8x6_SRGB_BLOCK,                            "VK_FORMAT_ASTC_8x6_SRGB_BLOCK"},
	{VK_FORMAT_ASTC_8x8_UNORM_BLOCK,                           "VK_FORMAT_ASTC_8x8_UNORM_BLOCK"},
	{VK_FORMAT_ASTC_8x8_SRGB_BLOCK,                            "VK_FORMAT_ASTC_8x8_SRGB_BLOCK"},
	{VK_FORMAT_ASTC_10x5_UNORM_BLOCK,                          "VK_FORMAT_ASTC_10x5_UNORM_BLOCK"},
	{VK_FORMAT_ASTC_10x5_SRGB_BLOCK,                           "VK_FORMAT_ASTC_10x5_SRGB_BLOCK"},
	{VK_FORMAT_ASTC_10x6_UNORM_BLOCK,                          "VK_FORMAT_ASTC_10x6_UNORM_BLOCK"},
	{VK_FORMAT_ASTC_10x6_SRGB_BLOCK,                           "VK_FORMAT_ASTC_10x6_SRGB_BLOCK"},
	{VK_FORMAT_ASTC_10x8_UNORM_BLOCK,                          "VK_FORMAT_ASTC_10x8_UNORM_BLOCK"},
	{VK_FORMAT_ASTC_10x8_SRGB_BLOCK,                           "VK_FORMAT_ASTC_10x8_SRGB_BLOCK"},
	{VK_FORMAT_ASTC_10x10_UNORM_BLOCK,                         "VK_FORMAT_ASTC_10x10_UNORM_BLOCK"},
	{VK_FORMAT_ASTC_10x10_SRGB_BLOCK,                          "VK_FORMAT_ASTC_10x10_SRGB_BLOCK"},
	{VK_FORMAT_ASTC_12x10_UNORM_BLOCK,                         "VK_FORMAT_ASTC_12x10_UNORM_BLOCK"},
	{VK_FORMAT_ASTC_12x10_SRGB_BLOCK,                          "VK_FORMAT_ASTC_12x10_SRGB_BLOCK"},
	{VK_FORMAT_ASTC_12x12_UNORM_BLOCK,                         "VK_FORMAT_ASTC_12x12_UNORM_BLOCK"},
	{VK_FORMAT_ASTC_12x12_SRGB_BLOCK,                          "VK_FORMAT_ASTC_12x12_SRGB_BLOCK"},
	{VK_FORMAT_G8B8G8R8_422_UNORM,                             "VK_FORMAT_G8B8G8R8_422_UNORM"},
	{VK_FORMAT_B8G8R8G8_422_UNORM,                             "VK_FORMAT_B8G8R8G8_422_UNORM"},
	{VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM,                      "VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM"},
	{VK_FORMAT_G8_B8R8_2PLANE_420_UNORM,                       "VK_FORMAT_G8_B8R8_2PLANE_420_UNORM"},
	{VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM,                      "VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM"},
	{VK_FORMAT_G8_B8R8_2PLANE_422_UNORM,                       "VK_FORMAT_G8_B8R8_2PLANE_422_UNORM"},
	{VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM,                      "VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM"},
	{VK_FORMAT_R10X6_UNORM_PACK16,                             "VK_FORMAT_R10X6_UNORM_PACK16"},
	{VK_FORMAT_R10X6G10X6_UNORM_2PACK16,                       "VK_FORMAT_R10X6G10X6_UNORM_2PACK16"},
	{VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16,             "VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16"},
	{VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16,         "VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16"},
	{VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16,         "VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16"},
	{VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16,     "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16"},
	{VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16,      "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16"},
	{VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16,     "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16"},
	{VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16,      "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16"},
	{VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16,     "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16"},
	{VK_FORMAT_R12X4_UNORM_PACK16,                             "VK_FORMAT_R12X4_UNORM_PACK16"},
	{VK_FORMAT_R12X4G12X4_UNORM_2PACK16,                       "VK_FORMAT_R12X4G12X4_UNORM_2PACK16"},
	{VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16,             "VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16"},
	{VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16,         "VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16"},
	{VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16,         "VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16"},
	{VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16,     "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16"},
	{VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16,      "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16"},
	{VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16,     "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16"},
	{VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16,      "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16"},
	{VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16,     "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16"},
	{VK_FORMAT_G16B16G16R16_422_UNORM,                         "VK_FORMAT_G16B16G16R16_422_UNORM"},
	{VK_FORMAT_B16G16R16G16_422_UNORM,                         "VK_FORMAT_B16G16R16G16_422_UNORM"},
	{VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM,                   "VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM"},
	{VK_FORMAT_G16_B16R16_2PLANE_420_UNORM,                    "VK_FORMAT_G16_B16R16_2PLANE_420_UNORM"},
	{VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM,                   "VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM"},
	{VK_FORMAT_G16_B16R16_2PLANE_422_UNORM,                    "VK_FORMAT_G16_B16R16_2PLANE_422_UNORM"},
	{VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM,                   "VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM"},
	{VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG,                    "VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG"},
	{VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG,                    "VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG"},
	{VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG,                    "VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG"},
	{VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG,                    "VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG"},
	{VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG,                     "VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG"},
	{VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG,                     "VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG"},
	{VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG,                     "VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG"},
	{VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG,                     "VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG"},
	{VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT,                      "VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT"},
	{VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT,                      "VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT"},
	{VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT,                      "VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT"},
	{VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT,                      "VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT"},
	{VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT,                      "VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT"},
	{VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT,                      "VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT"},
	{VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT,                      "VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT"},
	{VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT,                      "VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT"},
	{VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT,                     "VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT"},
	{VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT,                     "VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT"},
	{VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT,                     "VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT"},
	{VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT,                    "VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT"},
	{VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT,                    "VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT"},
	{VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT,                    "VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT"},
	{VK_FORMAT_G8B8G8R8_422_UNORM_KHR,                         "VK_FORMAT_G8B8G8R8_422_UNORM_KHR"},
	{VK_FORMAT_B8G8R8G8_422_UNORM_KHR,                         "VK_FORMAT_B8G8R8G8_422_UNORM_KHR"},
	{VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM_KHR,                  "VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM_KHR"},
	{VK_FORMAT_G8_B8R8_2PLANE_420_UNORM_KHR,                   "VK_FORMAT_G8_B8R8_2PLANE_420_UNORM_KHR"},
	{VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM_KHR,                  "VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM_KHR"},
	{VK_FORMAT_G8_B8R8_2PLANE_422_UNORM_KHR,                   "VK_FORMAT_G8_B8R8_2PLANE_422_UNORM_KHR"},
	{VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM_KHR,                  "VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM_KHR"},
	{VK_FORMAT_R10X6_UNORM_PACK16_KHR,                         "VK_FORMAT_R10X6_UNORM_PACK16_KHR"},
	{VK_FORMAT_R10X6G10X6_UNORM_2PACK16_KHR,                   "VK_FORMAT_R10X6G10X6_UNORM_2PACK16_KHR"},
	{VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR,         "VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR"},
	{VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR,     "VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR"},
	{VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR,     "VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR"},
	{VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR, "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR"},
	{VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR,  "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR"},
	{VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR, "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR"},
	{VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR,  "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR"},
	{VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR, "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR"},
	{VK_FORMAT_R12X4_UNORM_PACK16_KHR,                         "VK_FORMAT_R12X4_UNORM_PACK16_KHR"},
	{VK_FORMAT_R12X4G12X4_UNORM_2PACK16_KHR,                   "VK_FORMAT_R12X4G12X4_UNORM_2PACK16_KHR"},
	{VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR,         "VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR"},
	{VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR,     "VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR"},
	{VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR,     "VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR"},
	{VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR, "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR"},
	{VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR,  "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR"},
	{VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR, "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR"},
	{VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR,  "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR"},
	{VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR, "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR"},
	{VK_FORMAT_G16B16G16R16_422_UNORM_KHR,                     "VK_FORMAT_G16B16G16R16_422_UNORM_KHR"},
	{VK_FORMAT_B16G16R16G16_422_UNORM_KHR,                     "VK_FORMAT_B16G16R16G16_422_UNORM_KHR"},
	{VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR,               "VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR"},
	{VK_FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR,                "VK_FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR"},
	{VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR,               "VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR"},
	{VK_FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR,                "VK_FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR"},
	{VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR,               "VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR"},
};

std::unordered_map<std::underlying_type_t<VkPresentModeKHR>, std::string> present_mode_string {
	{VK_PRESENT_MODE_IMMEDIATE_KHR,                 "VK_PRESENT_MODE_IMMEDIATE_KHR"},
	{VK_PRESENT_MODE_MAILBOX_KHR,                   "VK_PRESENT_MODE_MAILBOX_KHR"},
	{VK_PRESENT_MODE_FIFO_KHR,                      "VK_PRESENT_MODE_FIFO_KHR"},
	{VK_PRESENT_MODE_FIFO_RELAXED_KHR,              "VK_PRESENT_MODE_FIFO_RELAXED_KHR"},
	{VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR,     "VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR"},
	{VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR, "VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR"},
};

std::unordered_map<std::underlying_type_t<VkColorSpaceKHR>, std::string> color_space_string {
	{VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,          "VK_COLOR_SPACE_SRGB_NONLINEAR_KHR"},
	{VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT,    "VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT"},
	{VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT,    "VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT"},
	{VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT,       "VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT"},
	{VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT,        "VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT"},
	{VK_COLOR_SPACE_BT709_LINEAR_EXT,            "VK_COLOR_SPACE_BT709_LINEAR_EXT"},
	{VK_COLOR_SPACE_BT709_NONLINEAR_EXT,         "VK_COLOR_SPACE_BT709_NONLINEAR_EXT"},
	{VK_COLOR_SPACE_BT2020_LINEAR_EXT,           "VK_COLOR_SPACE_BT2020_LINEAR_EXT"},
	{VK_COLOR_SPACE_HDR10_ST2084_EXT,            "VK_COLOR_SPACE_HDR10_ST2084_EXT"},
	{VK_COLOR_SPACE_DOLBYVISION_EXT,             "VK_COLOR_SPACE_DOLBYVISION_EXT"},
	{VK_COLOR_SPACE_HDR10_HLG_EXT,               "VK_COLOR_SPACE_HDR10_HLG_EXT"},
	{VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT,         "VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT"},
	{VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT,      "VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT"},
	{VK_COLOR_SPACE_PASS_THROUGH_EXT,            "VK_COLOR_SPACE_PASS_THROUGH_EXT"},
	{VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT, "VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT"},
	{VK_COLOR_SPACE_DISPLAY_NATIVE_AMD,          "VK_COLOR_SPACE_DISPLAY_NATIVE_AMD"},
	{VK_COLORSPACE_SRGB_NONLINEAR_KHR,           "VK_COLORSPACE_SRGB_NONLINEAR_KHR"},
	{VK_COLOR_SPACE_DCI_P3_LINEAR_EXT,           "VK_COLOR_SPACE_DCI_P3_LINEAR_EXT"},
};

#define concat2(a, b) a ## b
#define concat(a, b) concat2(a, b)
#define unique_name concat(unique_name_, __LINE__)
#define error                                                                      \
struct {                                                                           \
	void operator<< (VkResult error_code) {                                          \
		if (error_code != VK_SUCCESS) {                                                \
			std::cout << "fatal: vukan error (" << __FILE__ << ": " << __LINE__ << "): " \
				<< error_code << " ("<< error_string[error_code] << ")" << std::endl;      \
			std::abort();                                                                \
		}                                                                              \
	}                                                                                \
} unique_name; unique_name

void dumpPhysicalDeviceLimit(const VkPhysicalDeviceLimits& limits)
{
	std::cout << "\tlimits" << std::endl
		<< "\t\tmaxImageDimension1D:                             " << limits.maxImageDimension1D << std::endl
		<< "\t\tmaxImageDimension2D:                             " << limits.maxImageDimension2D << std::endl
		<< "\t\tmaxImageDimension3D:                             " << limits.maxImageDimension3D << std::endl
		<< "\t\tmaxImageDimensionCube:                           " << limits.maxImageDimensionCube << std::endl
		<< "\t\tmaxImageArrayLayers:                             " << limits.maxImageArrayLayers << std::endl
		<< "\t\tmaxTexelBufferElements:                          " << limits.maxTexelBufferElements << std::endl
		<< "\t\tmaxUniformBufferRange:                           " << limits.maxUniformBufferRange << std::endl
		<< "\t\tmaxStorageBufferRange:                           " << limits.maxStorageBufferRange << std::endl
		<< "\t\tmaxPushConstantsSize:                            " << limits.maxPushConstantsSize << std::endl
		<< "\t\tmaxMemoryAllocationCount:                        " << limits.maxMemoryAllocationCount << std::endl
		<< "\t\tmaxSamplerAllocationCount:                       " << limits.maxSamplerAllocationCount << std::endl
		<< "\t\tbufferImageGranularity:                          " << limits.bufferImageGranularity << std::endl
		<< "\t\tsparseAddressSpaceSize:                          " << limits.sparseAddressSpaceSize << std::endl
		<< "\t\tmaxBoundDescriptorSets:                          " << limits.maxBoundDescriptorSets << std::endl
		<< "\t\tmaxPerStageDescriptorSamplers:                   " << limits.maxPerStageDescriptorSamplers << std::endl
		<< "\t\tmaxPerStageDescriptorUniformBuffers:             " << limits.maxPerStageDescriptorUniformBuffers << std::endl
		<< "\t\tmaxPerStageDescriptorStorageBuffers:             " << limits.maxPerStageDescriptorStorageBuffers << std::endl
		<< "\t\tmaxPerStageDescriptorSampledImages:              " << limits.maxPerStageDescriptorSampledImages << std::endl
		<< "\t\tmaxPerStageDescriptorStorageImages:              " << limits.maxPerStageDescriptorStorageImages << std::endl
		<< "\t\tmaxPerStageDescriptorInputAttachments:           " << limits.maxPerStageDescriptorInputAttachments << std::endl
		<< "\t\tmaxPerStageResources:                            " << limits.maxPerStageResources << std::endl
		<< "\t\tmaxDescriptorSetSamplers:                        " << limits.maxDescriptorSetSamplers << std::endl
		<< "\t\tmaxDescriptorSetUniformBuffers:                  " << limits.maxDescriptorSetUniformBuffers << std::endl
		<< "\t\tmaxDescriptorSetUniformBuffersDynamic:           " << limits.maxDescriptorSetUniformBuffersDynamic << std::endl
		<< "\t\tmaxDescriptorSetStorageBuffers:                  " << limits.maxDescriptorSetStorageBuffers << std::endl
		<< "\t\tmaxDescriptorSetStorageBuffersDynamic:           " << limits.maxDescriptorSetStorageBuffersDynamic << std::endl
		<< "\t\tmaxDescriptorSetSampledImages:                   " << limits.maxDescriptorSetSampledImages << std::endl
		<< "\t\tmaxDescriptorSetStorageImages:                   " << limits.maxDescriptorSetStorageImages << std::endl
		<< "\t\tmaxDescriptorSetInputAttachments:                " << limits.maxDescriptorSetInputAttachments << std::endl
		<< "\t\tmaxVertexInputAttributes:                        " << limits.maxVertexInputAttributes << std::endl
		<< "\t\tmaxVertexInputBindings:                          " << limits.maxVertexInputBindings << std::endl
		<< "\t\tmaxVertexInputAttributeOffset:                   " << limits.maxVertexInputAttributeOffset << std::endl
		<< "\t\tmaxVertexInputBindingStride:                     " << limits.maxVertexInputBindingStride << std::endl
		<< "\t\tmaxVertexOutputComponents:                       " << limits.maxVertexOutputComponents << std::endl
		<< "\t\tmaxTessellationGenerationLevel:                  " << limits.maxTessellationGenerationLevel << std::endl
		<< "\t\tmaxTessellationPatchSize:                        " << limits.maxTessellationPatchSize << std::endl
		<< "\t\tmaxTessellationControlPerVertexInputComponents:  " << limits.maxTessellationControlPerVertexInputComponents << std::endl
		<< "\t\tmaxTessellationControlPerVertexOutputComponents: " << limits.maxTessellationControlPerVertexOutputComponents << std::endl
		<< "\t\tmaxTessellationControlPerPatchOutputComponents:  " << limits.maxTessellationControlPerPatchOutputComponents << std::endl
		<< "\t\tmaxTessellationControlTotalOutputComponents:     " << limits.maxTessellationControlTotalOutputComponents << std::endl
		<< "\t\tmaxTessellationEvaluationInputComponents:        " << limits.maxTessellationEvaluationInputComponents << std::endl
		<< "\t\tmaxTessellationEvaluationOutputComponents:       " << limits.maxTessellationEvaluationOutputComponents << std::endl
		<< "\t\tmaxGeometryShaderInvocations:                    " << limits.maxGeometryShaderInvocations << std::endl
		<< "\t\tmaxGeometryInputComponents:                      " << limits.maxGeometryInputComponents << std::endl
		<< "\t\tmaxGeometryOutputComponents:                     " << limits.maxGeometryOutputComponents << std::endl
		<< "\t\tmaxGeometryOutputVertices:                       " << limits.maxGeometryOutputVertices << std::endl
		<< "\t\tmaxGeometryTotalOutputComponents:                " << limits.maxGeometryTotalOutputComponents << std::endl
		<< "\t\tmaxFragmentInputComponents:                      " << limits.maxFragmentInputComponents << std::endl
		<< "\t\tmaxFragmentOutputAttachments:                    " << limits.maxFragmentOutputAttachments << std::endl
		<< "\t\tmaxFragmentDualSrcAttachments:                   " << limits.maxFragmentDualSrcAttachments << std::endl
		<< "\t\tmaxFragmentCombinedOutputResources:              " << limits.maxFragmentCombinedOutputResources << std::endl
		<< "\t\tmaxComputeSharedMemorySize:                      " << limits.maxComputeSharedMemorySize << std::endl
		<< "\t\tmaxComputeWorkGroupCount[3]:                     " 
			<< limits.maxComputeWorkGroupCount[0] << ", "
			<< limits.maxComputeWorkGroupCount[1] << ", "
			<< limits.maxComputeWorkGroupCount[2] << std::endl
		<< "\t\tmaxComputeWorkGroupInvocations:                  " << limits.maxComputeWorkGroupInvocations << std::endl
		<< "\t\tmaxComputeWorkGroupSize[3]:                      "
			<< limits.maxComputeWorkGroupSize[0] << ", "
			<< limits.maxComputeWorkGroupSize[1] << ", "
			<< limits.maxComputeWorkGroupSize[2] << std::endl
		<< "\t\tsubPixelPrecisionBits:                           " << limits.subPixelPrecisionBits << std::endl
		<< "\t\tsubTexelPrecisionBits:                           " << limits.subTexelPrecisionBits << std::endl
		<< "\t\tmipmapPrecisionBits:                             " << limits.mipmapPrecisionBits << std::endl
		<< "\t\tmaxDrawIndexedIndexValue:                        " << limits.maxDrawIndexedIndexValue << std::endl
		<< "\t\tmaxDrawIndirectCount:                            " << limits.maxDrawIndirectCount << std::endl
		<< "\t\tmaxSamplerLodBias:                               " << limits.maxSamplerLodBias << std::endl
		<< "\t\tmaxSamplerAnisotropy:                            " << limits.maxSamplerAnisotropy << std::endl
		<< "\t\tmaxViewports:                                    " << limits.maxViewports << std::endl
		<< "\t\tmaxViewportDimensions[2]:                        " 
			<< limits.maxViewportDimensions[0] << ", "
			<< limits.maxViewportDimensions[1] << std::endl
		<< "\t\tviewportBoundsRange[2]:                          " 
			<< limits.viewportBoundsRange[0] << ", "
			<< limits.viewportBoundsRange[1] << std::endl
		<< "\t\tviewportSubPixelBits:                            " << limits.viewportSubPixelBits << std::endl
		<< "\t\tminMemoryMapAlignment:                           " << limits.minMemoryMapAlignment << std::endl
		<< "\t\tminTexelBufferOffsetAlignment:                   " << limits.minTexelBufferOffsetAlignment << std::endl
		<< "\t\tminUniformBufferOffsetAlignment:                 " << limits.minUniformBufferOffsetAlignment << std::endl
		<< "\t\tminStorageBufferOffsetAlignment:                 " << limits.minStorageBufferOffsetAlignment << std::endl
		<< "\t\tminTexelOffset:                                  " << limits.minTexelOffset << std::endl
		<< "\t\tmaxTexelOffset:                                  " << limits.maxTexelOffset << std::endl
		<< "\t\tminTexelGatherOffset:                            " << limits.minTexelGatherOffset << std::endl
		<< "\t\tmaxTexelGatherOffset:                            " << limits.maxTexelGatherOffset << std::endl
		<< "\t\tminInterpolationOffset:                          " << limits.minInterpolationOffset << std::endl
		<< "\t\tmaxInterpolationOffset:                          " << limits.maxInterpolationOffset << std::endl
		<< "\t\tsubPixelInterpolationOffsetBits:                 " << limits.subPixelInterpolationOffsetBits << std::endl
		<< "\t\tmaxFramebufferWidth:                             " << limits.maxFramebufferWidth << std::endl
		<< "\t\tmaxFramebufferHeight:                            " << limits.maxFramebufferHeight << std::endl
		<< "\t\tmaxFramebufferLayers:                            " << limits.maxFramebufferLayers << std::endl
		<< "\t\tframebufferColorSampleCounts:                    " << limits.framebufferColorSampleCounts << std::endl
		<< "\t\tframebufferDepthSampleCounts:                    " << limits.framebufferDepthSampleCounts << std::endl
		<< "\t\tframebufferStencilSampleCounts:                  " << limits.framebufferStencilSampleCounts << std::endl
		<< "\t\tframebufferNoAttachmentsSampleCounts:            " << limits.framebufferNoAttachmentsSampleCounts << std::endl
		<< "\t\tmaxColorAttachments:                             " << limits.maxColorAttachments << std::endl
		<< "\t\tsampledImageColorSampleCounts:                   " << limits.sampledImageColorSampleCounts << std::endl
		<< "\t\tsampledImageIntegerSampleCounts:                 " << limits.sampledImageIntegerSampleCounts << std::endl
		<< "\t\tsampledImageDepthSampleCounts:                   " << limits.sampledImageDepthSampleCounts << std::endl
		<< "\t\tsampledImageStencilSampleCounts:                 " << limits.sampledImageStencilSampleCounts << std::endl
		<< "\t\tstorageImageSampleCounts:                        " << limits.storageImageSampleCounts << std::endl
		<< "\t\tmaxSampleMaskWords:                              " << limits.maxSampleMaskWords << std::endl
		<< "\t\ttimestampComputeAndGraphics:                     " << limits.timestampComputeAndGraphics << std::endl
		<< "\t\ttimestampPeriod:                                 " << limits.timestampPeriod << std::endl
		<< "\t\tmaxClipDistances:                                " << limits.maxClipDistances << std::endl
		<< "\t\tmaxCullDistances:                                " << limits.maxCullDistances << std::endl
		<< "\t\tmaxCombinedClipAndCullDistances:                 " << limits.maxCombinedClipAndCullDistances << std::endl
		<< "\t\tdiscreteQueuePriorities:                         " << limits.discreteQueuePriorities << std::endl
		<< "\t\tpointSizeRange[2]:                               " 
			<< limits.pointSizeRange[0] << ", "
			<< limits.pointSizeRange[1] << std::endl
		<< "\t\tlineWidthRange[2]:                               " 
			<< limits.lineWidthRange[0] << ", "
			<< limits.lineWidthRange[0] << std::endl
		<< "\t\tpointSizeGranularity:                            " << limits.pointSizeGranularity << std::endl
		<< "\t\tlineWidthGranularity:                            " << limits.lineWidthGranularity << std::endl
		<< "\t\tstrictLines:                                     " << limits.strictLines << std::endl
		<< "\t\tstandardSampleLocations:                         " << limits.standardSampleLocations << std::endl
		<< "\t\toptimalBufferCopyOffsetAlignment:                " << limits.optimalBufferCopyOffsetAlignment << std::endl
		<< "\t\toptimalBufferCopyRowPitchAlignment:              " << limits.optimalBufferCopyRowPitchAlignment << std::endl
		<< "\t\tnonCoherentAtomSize:                             " << limits.nonCoherentAtomSize << std::endl;
}

void dumpPhysicalDeviceFeatures(const VkPhysicalDeviceFeatures& features)
{
	std::cout << "\tfeatures" << std::endl
		<< "\t\trobustBufferAccess:                      " << features.robustBufferAccess << std::endl
		<< "\t\tfullDrawIndexUint32:                     " << features.fullDrawIndexUint32 << std::endl
		<< "\t\timageCubeArray:                          " << features.imageCubeArray << std::endl
		<< "\t\tindependentBlend:                        " << features.independentBlend << std::endl
		<< "\t\tgeometryShader:                          " << features.geometryShader << std::endl
		<< "\t\ttessellationShader:                      " << features.tessellationShader << std::endl
		<< "\t\tsampleRateShading:                       " << features.sampleRateShading << std::endl
		<< "\t\tdualSrcBlend:                            " << features.dualSrcBlend << std::endl
		<< "\t\tlogicOp:                                 " << features.logicOp << std::endl
		<< "\t\tmultiDrawIndirect:                       " << features.multiDrawIndirect << std::endl
		<< "\t\tdrawIndirectFirstInstance:               " << features.drawIndirectFirstInstance << std::endl
		<< "\t\tdepthClamp:                              " << features.depthClamp << std::endl
		<< "\t\tdepthBiasClamp:                          " << features.depthBiasClamp << std::endl
		<< "\t\tfillModeNonSolid:                        " << features.fillModeNonSolid << std::endl
		<< "\t\tdepthBounds:                             " << features.depthBounds << std::endl
		<< "\t\twideLines:                               " << features.wideLines << std::endl
		<< "\t\tlargePoints:                             " << features.largePoints << std::endl
		<< "\t\talphaToOne:                              " << features.alphaToOne << std::endl
		<< "\t\tmultiViewport:                           " << features.multiViewport << std::endl
		<< "\t\tsamplerAnisotropy:                       " << features.samplerAnisotropy << std::endl
		<< "\t\ttextureCompressionETC2:                  " << features.textureCompressionETC2 << std::endl
		<< "\t\ttextureCompressionASTC_LDR:              " << features.textureCompressionASTC_LDR << std::endl
		<< "\t\ttextureCompressionBC:                    " << features.textureCompressionBC << std::endl
		<< "\t\tocclusionQueryPrecise:                   " << features.occlusionQueryPrecise << std::endl
		<< "\t\tpipelineStatisticsQuery:                 " << features.pipelineStatisticsQuery << std::endl
		<< "\t\tvertexPipelineStoresAndAtomics:          " << features.vertexPipelineStoresAndAtomics << std::endl
		<< "\t\tfragmentStoresAndAtomics:                " << features.fragmentStoresAndAtomics << std::endl
		<< "\t\tshaderTessellationAndGeometryPointSize:  " << features.shaderTessellationAndGeometryPointSize << std::endl
		<< "\t\tshaderImageGatherExtended:               " << features.shaderImageGatherExtended << std::endl
		<< "\t\tshaderStorageImageExtendedFormats:       " << features.shaderStorageImageExtendedFormats << std::endl
		<< "\t\tshaderStorageImageMultisample:           " << features.shaderStorageImageMultisample << std::endl
		<< "\t\tshaderStorageImageReadWithoutFormat:     " << features.shaderStorageImageReadWithoutFormat << std::endl
		<< "\t\tshaderStorageImageWriteWithoutFormat:    " << features.shaderStorageImageWriteWithoutFormat << std::endl
		<< "\t\tshaderUniformBufferArrayDynamicIndexing: " << features.shaderUniformBufferArrayDynamicIndexing << std::endl
		<< "\t\tshaderSampledImageArrayDynamicIndexing:  " << features.shaderSampledImageArrayDynamicIndexing << std::endl
		<< "\t\tshaderStorageBufferArrayDynamicIndexing: " << features.shaderStorageBufferArrayDynamicIndexing << std::endl
		<< "\t\tshaderStorageImageArrayDynamicIndexing:  " << features.shaderStorageImageArrayDynamicIndexing << std::endl
		<< "\t\tshaderClipDistance:                      " << features.shaderClipDistance << std::endl
		<< "\t\tshaderCullDistance:                      " << features.shaderCullDistance << std::endl
		<< "\t\tshaderFloat64:                           " << features.shaderFloat64 << std::endl
		<< "\t\tshaderInt64:                             " << features.shaderInt64 << std::endl
		<< "\t\tshaderInt16:                             " << features.shaderInt16 << std::endl
		<< "\t\tshaderResourceResidency:                 " << features.shaderResourceResidency << std::endl
		<< "\t\tshaderResourceMinLod:                    " << features.shaderResourceMinLod << std::endl
		<< "\t\tsparseBinding:                           " << features.sparseBinding << std::endl
		<< "\t\tsparseResidencyBuffer:                   " << features.sparseResidencyBuffer << std::endl
		<< "\t\tsparseResidencyImage2D:                  " << features.sparseResidencyImage2D << std::endl
		<< "\t\tsparseResidencyImage3D:                  " << features.sparseResidencyImage3D << std::endl
		<< "\t\tsparseResidency2Samples:                 " << features.sparseResidency2Samples << std::endl
		<< "\t\tsparseResidency4Samples:                 " << features.sparseResidency4Samples << std::endl
		<< "\t\tsparseResidency8Samples:                 " << features.sparseResidency8Samples << std::endl
		<< "\t\tsparseResidency16Samples:                " << features.sparseResidency16Samples << std::endl
		<< "\t\tsparseResidencyAliased:                  " << features.sparseResidencyAliased << std::endl
		<< "\t\tvariableMultisampleRate:                 " << features.variableMultisampleRate << std::endl
		<< "\t\tinheritedQueries:                        " << features.inheritedQueries << std::endl;
}

void dumpPhysicalDevice(const VkPhysicalDevice& device)
{
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(device, &properties);
	std::cout << "physical device" << std::endl
		<< "\tapi version: "
			<< VK_VERSION_MAJOR(properties.apiVersion) << "."
			<< VK_VERSION_MINOR(properties.apiVersion) << "."
			<< VK_VERSION_PATCH(properties.apiVersion) << std::endl
		<< "\tdiver version:      " << properties.driverVersion << std::endl
		<< "\tvendor id:          " << properties.vendorID << std::endl
		<< "\tdevice id:          " << properties.deviceID << std::endl
		<< "\tdevice type:        " << properties.deviceType << " (" << physical_device_type_name[properties.deviceType] << ")" << std::endl
		<< "\tdevice name:        " << properties.deviceName << std::endl
		<< "\tpipeline cach uuid: " << properties.pipelineCacheUUID << std::endl;
		dumpPhysicalDeviceLimit(properties.limits);
		// std::cout << "\tsparse properties: " << "..." << std::endl; // properties.sparseProperties
	
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(device, &features);
	dumpPhysicalDeviceFeatures(features);

	VkPhysicalDeviceMemoryProperties memory;
	vkGetPhysicalDeviceMemoryProperties(device, &memory);
	std::cout << "\tmemory types [" << memory.memoryTypeCount << "]" << std::endl;
	for (std::size_t i = 0; i < memory.memoryTypeCount; ++i) {
		const auto& type = memory.memoryTypes[i];
		std::cout << "\t\t#" << i << std::endl
			<< "\t\theap index:                                 " << type.heapIndex << std::endl
			<< "\t\tVK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT:        " << (0 != (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT & type.propertyFlags)) << std::endl
			<< "\t\tVK_MEMORY_PROPERTY_HOST_VISIBLE_BIT:        " << (0 != (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT & type.propertyFlags)) << std::endl
			<< "\t\tVK_MEMORY_PROPERTY_HOST_COHERENT_BIT:       " << (0 != (VK_MEMORY_PROPERTY_HOST_COHERENT_BIT & type.propertyFlags)) << std::endl
			<< "\t\tVK_MEMORY_PROPERTY_HOST_CACHED_BIT:         " << (0 != (VK_MEMORY_PROPERTY_HOST_CACHED_BIT & type.propertyFlags)) << std::endl
			<< "\t\tVK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT:    " << (0 != (VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT & type.propertyFlags)) << std::endl
			<< "\t\tVK_MEMORY_PROPERTY_PROTECTED_BIT:           " << (0 != (VK_MEMORY_PROPERTY_PROTECTED_BIT & type.propertyFlags)) << std::endl
			<< "\t\tVK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD: " << (0 != (VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD & type.propertyFlags)) << std::endl
			<< "\t\tVK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD: " << (0 != (VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD & type.propertyFlags)) << std::endl
			<< "\t\tVK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM:      " << (0 != (VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM & type.propertyFlags)) << std::endl;
	}

	std::uint32_t familyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);
	auto families = std::vector<VkQueueFamilyProperties>(familyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, families.data());

	std::cout << "\tqueue families [" << familyCount << "]" << std::endl;
	for (std::size_t i = 0; i < familyCount; ++i) {
		const auto& family = families[i];
		std::cout << "\t\t#" << i << std::endl
			<< "\t\tqueue count:                     " << family.queueCount << std::endl
			<< "\t\ttimestamp valid bits:            " << family.timestampValidBits << std::endl
			<< "\t\tmin image timestamp granularity: "
				<< family.minImageTransferGranularity.width << ", "
				<< family.minImageTransferGranularity.height << ", "
				<< family.minImageTransferGranularity.depth << std::endl
			<< "\t\tVK_QUEUE_GRAPHICS_BIT:           " << (0 != (VK_QUEUE_GRAPHICS_BIT & family.queueFlags)) << std::endl
			<< "\t\tVK_QUEUE_COMPUTE_BIT:            " << (0 != (VK_QUEUE_COMPUTE_BIT & family.queueFlags)) << std::endl
			<< "\t\tVK_QUEUE_TRANSFER_BIT:           " << (0 != (VK_QUEUE_TRANSFER_BIT & family.queueFlags)) << std::endl
			<< "\t\tVK_QUEUE_SPARSE_BINDING_BIT:     " << (0 != (VK_QUEUE_SPARSE_BINDING_BIT & family.queueFlags)) << std::endl
			<< "\t\tVK_QUEUE_PROTECTED_BIT:          " << (0 != (VK_QUEUE_PROTECTED_BIT & family.queueFlags)) << std::endl;
	}
}

void dumpLayer(const VkLayerProperties &layer)
{
	std::cout << "layer" << std::endl
		<< "\tname:                   " << layer.layerName << std::endl
		<< "\tspecification version:  " << layer.specVersion << std::endl
		<< "\timplementation version: " << layer.implementationVersion << std::endl
		<< "\tdescription:            " << layer.description << std::endl;
}

void dumpExtension(const VkExtensionProperties &extension)
{
	std::cout << "extension" << std::endl
		<< "\tname:                  " << extension.extensionName << std::endl
		<< "\tspecification version: " << extension.specVersion << std::endl;
}

void dumpSurfaceCapabilities(const VkSurfaceCapabilitiesKHR& capabilities)
{
	std::cout << "\tsurface" << std::endl
		<< "\t\tminImageCount:           " << capabilities.minImageCount << std::endl
  	<< "\t\tmaxImageCount:           " << capabilities.maxImageCount << std::endl
  	<< "\t\tcurrentExtent:           "
			<< capabilities.currentExtent.width << ", "
			<< capabilities.currentExtent.height << std::endl
  	<< "\t\tminImageExtent:          "
			<< capabilities.minImageExtent.width << ", "
			<< capabilities.minImageExtent.height << std::endl
  	<< "\t\tmaxImageExtent:          "
			<< capabilities.maxImageExtent.width << ", "
			<< capabilities.maxImageExtent.height << std::endl
  	<< "\t\tmaxImageArrayLayers:     " << capabilities.maxImageArrayLayers << std::endl
  	<< "\t\tsupportedTransforms:     " << capabilities.supportedTransforms << std::endl
  	<< "\t\tcurrentTransform:        " << capabilities.currentTransform << std::endl
  	<< "\t\tsupportedCompositeAlpha: " << capabilities.supportedCompositeAlpha << std::endl
  	<< "\t\tsupportedUsageFlags:     " << capabilities.supportedUsageFlags << std::endl;
}

using ShaderCodeChar = std::uint32_t;
using ShaderCode = std::vector<ShaderCodeChar>;

ShaderCode readShaderFile(const std::filesystem::path& filepath)
{
	std::ifstream file(filepath, std::ios::binary | std::ios::ate);
	assert(file);
	std::size_t size = file.tellg();
	assert(size % sizeof(ShaderCodeChar) == 0);
	file.seekg(0);
	auto content = ShaderCode(size / sizeof(ShaderCodeChar));
	file.read(reinterpret_cast<char*>(content.data()), size);
	return content;
}

VkShaderModule createShader(VkDevice device, const ShaderCode& code)
{
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

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	std::cout << std::boolalpha;
	const std::size_t window_width = 1280, window_height = 720;
	VkFormat image_format = VK_FORMAT_B8G8R8A8_UNORM;

	assert(glfwInit() == GLFW_TRUE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Vulkan", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	VkApplicationInfo appInfo;
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pNext = nullptr;
  appInfo.pApplicationName = "Generic Sauce Vulkan Application";
  appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
  appInfo.pEngineName = "Generic Sauce Vulkan Graphics Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
  appInfo.apiVersion = VK_VERSION_1_0;

	std::uint32_t layerCount;
	error << vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	auto layerProperties = std::vector<VkLayerProperties>(layerCount);
	error << vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.data());
	std::cout << "layer count: " << layerCount << std::endl;
	for (const auto& layer : layerProperties)
		dumpLayer(layer);

	// TODO: choose layers nicely. maybe eliminate unavailable layers
	auto layers = std::vector<const char*>() = {
		"VK_LAYER_KHRONOS_validation"
	};

	std::cout << "choosing layers" << std::endl;
	for (const auto& name : layers)
		std::cout << "\t" << name << std::endl;

	std::uint32_t extensionCount;
	error << vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	auto extensionProperties = std::vector<VkExtensionProperties>(extensionCount);
	error << vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data());
	std::cout << "extension count: " << extensionCount << std::endl;
	for (const auto& extension : extensionProperties)
		dumpExtension(extension);

	// TODO: choose extensions
	auto extensions = std::vector<const char*>() = {};

	std::uint32_t glfwExtensionCount;
	auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	for (std::size_t i = 0; i < glfwExtensionCount; ++i)
		extensions.push_back(glfwExtensions[i]);

	std::cout << "choosing extensions" << std::endl;
	for (const auto& name : extensions)
		std::cout << "\t" << name << std::endl;

	VkInstanceCreateInfo instanceInfo;
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext = nullptr;
	instanceInfo.flags = 0;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledLayerCount = layers.size();
	instanceInfo.ppEnabledLayerNames = layers.data();
	instanceInfo.enabledExtensionCount = extensions.size();
	instanceInfo.ppEnabledExtensionNames = extensions.data();

	VkInstance instance;
	error << vkCreateInstance(&instanceInfo, nullptr, &instance);

	std::uint32_t physicalDeviceCount;
	error << vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
	auto physicalDevices = std::vector<VkPhysicalDevice>(physicalDeviceCount);
	error << vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

	for (const auto& device : physicalDevices)
		dumpPhysicalDevice(device);

	VkDeviceQueueCreateInfo queueInfo;
	queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfo.pNext = nullptr;
  queueInfo.flags = 0;
  queueInfo.queueFamilyIndex = 0; // TODO: choose best family
  queueInfo.queueCount = 1; // TODO: check if amount is available
	auto queuePriorities = std::vector<float> {1.f};
  queueInfo.pQueuePriorities = queuePriorities.data();

	VkPhysicalDeviceFeatures features = {};
	
	auto physicalDevice = physicalDevices[0]; // TODO: choose best device

	auto deviceExtensions = std::vector<const char*> {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkDeviceCreateInfo deviceInfo;
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceInfo.pNext = nullptr;
  deviceInfo.flags = 0;
  deviceInfo.queueCreateInfoCount = 1;
  deviceInfo.pQueueCreateInfos = &queueInfo;
  deviceInfo.enabledLayerCount = 0;
  deviceInfo.ppEnabledLayerNames = nullptr;
  deviceInfo.enabledExtensionCount = deviceExtensions.size();
  deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
  deviceInfo.pEnabledFeatures = &features;

	VkDevice device;
	error << vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device);

	VkSurfaceKHR surface;
	error << glfwCreateWindowSurface(instance, window, nullptr, &surface);
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	error << vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
	dumpSurfaceCapabilities(surfaceCapabilities);

	std::uint32_t formatCount;
	error << vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
	auto formats = std::vector<VkSurfaceFormatKHR>(formatCount);
	error << vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());
	std::cout << "\t\tformat count: " << formatCount << std::endl;
	for (const auto& format : formats) {
		std::cout << "\t\t\t" << format.format << " (" << format_string[format.format] << "), "
			<< format.colorSpace << " (" << color_space_string[format.colorSpace] << ")"<< std::endl;
	}

	std::uint32_t presentModeCount;
	error << vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
	auto presentModes = std::vector<VkPresentModeKHR>(presentModeCount);
	error << vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());
	std::cout << "\t\tpresent modes count: " << presentModeCount << std::endl;
	for (const auto& mode : presentModes)
		std::cout << "\t\t\t" << mode << " (" << present_mode_string[mode] << ")" << std::endl;

	VkSwapchainCreateInfoKHR swapchainInfo;
	swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchainInfo.pNext = nullptr;
  swapchainInfo.flags = 0;
  swapchainInfo.surface = surface;
  swapchainInfo.minImageCount = 2; // TODO: dependent
  swapchainInfo.imageFormat = image_format; // TODO: dependent
  swapchainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; // TODO: dependent
  swapchainInfo.imageExtent = VkExtent2D{window_width, window_height}; // TODO: dependent
  swapchainInfo.imageArrayLayers = 1;
  swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  swapchainInfo.queueFamilyIndexCount = 0;
  swapchainInfo.pQueueFamilyIndices = nullptr;
  swapchainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapchainInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR; // TODO: dependent
  swapchainInfo.clipped = VK_TRUE;
  swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

	VkBool32 surfaceSupport;
	error << vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, 0, surface, &surfaceSupport);
	assert(surfaceSupport == VK_TRUE);

	VkSwapchainKHR swapchain;
	error << vkCreateSwapchainKHR(device, &swapchainInfo, nullptr, &swapchain);

	std::uint32_t imageCount;
	error << vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
	auto images = std::vector<VkImage>(imageCount);
	error << vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data());

	auto imageViews = std::vector<VkImageView>(imageCount);
	for (std::size_t i = 0; i < imageCount; ++i) {
		VkImageViewCreateInfo imageViewInfo;
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.pNext = nullptr;
		imageViewInfo.flags = 0;
		imageViewInfo.image = images[i];
		imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format = image_format; 
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = 1;

		error << vkCreateImageView(device, &imageViewInfo, nullptr, &imageViews[i]);
	}
	std::cout << imageViews.size() << " image viws created" << std::endl;

	auto shaderCodeVert = readShaderFile("res/shader.vert.spv");
	auto shaderCodeFrag = readShaderFile("res/shader.frag.spv");

	auto shaderVert = createShader(device, shaderCodeVert);
	auto shaderFrag = createShader(device, shaderCodeFrag);

	VkPipelineShaderStageCreateInfo shaderStageInfoVert;
	shaderStageInfoVert.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfoVert.pNext = nullptr;
	shaderStageInfoVert.flags = 0;
	shaderStageInfoVert.stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStageInfoVert.module = shaderVert;
	shaderStageInfoVert.pName = "main";
	shaderStageInfoVert.pSpecializationInfo = nullptr;

	VkPipelineShaderStageCreateInfo shaderStageInfoFrag;
	shaderStageInfoFrag.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfoFrag.pNext = nullptr;
	shaderStageInfoFrag.flags = 0;
	shaderStageInfoFrag.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStageInfoFrag.module = shaderFrag;
	shaderStageInfoFrag.pName = "main";
	shaderStageInfoFrag.pSpecializationInfo = nullptr;

	auto shaderStageInfos = std::vector<VkPipelineShaderStageCreateInfo> {shaderStageInfoVert, shaderStageInfoFrag};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.pNext = nullptr;
	vertexInputInfo.flags = 0;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexBindingDescriptions = nullptr;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr;

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.pNext = nullptr;
	inputAssemblyInfo.flags = 0;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = window_width;
	viewport.height = window_height;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;

	VkRect2D scissor;
	scissor.offset = {0, 0};
	scissor.extent = {window_width, window_height};

	VkPipelineViewportStateCreateInfo viewportStateInfo;
	viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateInfo.pNext = nullptr;
	viewportStateInfo.flags = 0;
	viewportStateInfo.viewportCount = 1;
	viewportStateInfo.pViewports = &viewport;
	viewportStateInfo.scissorCount = 1;
	viewportStateInfo.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizationInfo.pNext = nullptr;
  rasterizationInfo.flags = 0;
  rasterizationInfo.depthClampEnable = VK_FALSE;
  rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
  rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizationInfo.depthBiasEnable = VK_FALSE;
  rasterizationInfo.depthBiasConstantFactor = 0.f;
  rasterizationInfo.depthBiasClamp = 0.f;
  rasterizationInfo.depthBiasSlopeFactor = 0.f;
  rasterizationInfo.lineWidth = 1.f;

	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampleInfo.pNext = nullptr;
  multisampleInfo.flags = 0;
  multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampleInfo.sampleShadingEnable = VK_FALSE;
  multisampleInfo.minSampleShading = 1.f;
  multisampleInfo.pSampleMask = nullptr;
  multisampleInfo.alphaToCoverageEnable = VK_FALSE;
  multisampleInfo.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendInfo.pNext = nullptr;
	colorBlendInfo.flags = 0;
	colorBlendInfo.logicOpEnable = VK_FALSE;
	colorBlendInfo.logicOp = VK_LOGIC_OP_NO_OP;
	colorBlendInfo.attachmentCount = 1;
	colorBlendInfo.pAttachments = &colorBlendAttachment;
	colorBlendInfo.blendConstants[0] = 0.f;
	colorBlendInfo.blendConstants[1] = 0.f;
	colorBlendInfo.blendConstants[2] = 0.f;
	colorBlendInfo.blendConstants[3] = 0.f;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.pNext = nullptr;
  pipelineLayoutInfo.flags = 0;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;

	VkPipelineLayout pipelineLayout;
	error << vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout);

	VkAttachmentDescription attachmentDescription;
	attachmentDescription.flags = 0;
	attachmentDescription.format = image_format;
	attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference attachmentReference;
	attachmentReference.attachment = 0;
	attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription;
	subpassDescription.flags = 0;
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.inputAttachmentCount = 0;
	subpassDescription.pInputAttachments = nullptr;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &attachmentReference;
	subpassDescription.pResolveAttachments = nullptr;
	subpassDescription.pDepthStencilAttachment = nullptr;
	subpassDescription.preserveAttachmentCount = 0;
	subpassDescription.pPreserveAttachments = nullptr;

	VkSubpassDependency subpassDependency;
	subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  subpassDependency.dstSubpass = 0;
  subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  subpassDependency.srcAccessMask = 0;
  subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  subpassDependency.dependencyFlags = 0;

	VkRenderPassCreateInfo renderPassInfo;
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.pNext = nullptr;
  renderPassInfo.flags = 0;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &attachmentDescription;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpassDescription;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &subpassDependency;

	VkRenderPass renderPass;
	error << vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass);

	VkGraphicsPipelineCreateInfo graphicsPipelineInfo;
	graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  graphicsPipelineInfo.pNext = nullptr;
  graphicsPipelineInfo.flags = 0;
  graphicsPipelineInfo.stageCount = 2;
  graphicsPipelineInfo.pStages = shaderStageInfos.data();
  graphicsPipelineInfo.pVertexInputState = &vertexInputInfo;
  graphicsPipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
  graphicsPipelineInfo.pTessellationState = nullptr;
  graphicsPipelineInfo.pViewportState = &viewportStateInfo;
  graphicsPipelineInfo.pRasterizationState = &rasterizationInfo;
  graphicsPipelineInfo.pMultisampleState = &multisampleInfo;
  graphicsPipelineInfo.pDepthStencilState = nullptr;
  graphicsPipelineInfo.pColorBlendState = &colorBlendInfo;
  graphicsPipelineInfo.pDynamicState = nullptr;
  graphicsPipelineInfo.layout = pipelineLayout;
  graphicsPipelineInfo.renderPass = renderPass;
  graphicsPipelineInfo.subpass = 0;
  graphicsPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  graphicsPipelineInfo.basePipelineIndex = -1;

	VkPipeline graphicsPipeline;
	error << vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &graphicsPipelineInfo, nullptr, &graphicsPipeline);

	auto framebuffers = std::vector<VkFramebuffer>(imageCount);
	for (std::size_t i = 0; i < imageCount; ++i) {
		VkFramebufferCreateInfo framebufferInfo;
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.pNext = nullptr;
    framebufferInfo.flags = 0;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &imageViews[i];
    framebufferInfo.width = window_width;
    framebufferInfo.height = window_height;
    framebufferInfo.layers = 1;

		error << vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffers[i]);
	}

	VkCommandPoolCreateInfo commandPoolInfo;
	commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  commandPoolInfo.pNext = nullptr;
  commandPoolInfo.flags = 0;
  commandPoolInfo.queueFamilyIndex = 0; // TODO: dependent

	VkCommandPool commandPool;
	error << vkCreateCommandPool(device, &commandPoolInfo, nullptr, &commandPool);

	VkCommandBufferAllocateInfo commandBufferInfo;
	commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferInfo.pNext = nullptr;
  commandBufferInfo.commandPool = commandPool;
  commandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  commandBufferInfo.commandBufferCount = imageCount;

	auto commandBuffers = std::vector<VkCommandBuffer>(imageCount);
	error << vkAllocateCommandBuffers(device, &commandBufferInfo, commandBuffers.data());

	VkCommandBufferBeginInfo beginInfo;
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.pNext = nullptr;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
  beginInfo.pInheritanceInfo = nullptr;

	for (std::size_t i = 0; i < imageCount; ++i) {
		error << vkBeginCommandBuffer(commandBuffers[i], &beginInfo);

		VkRenderPassBeginInfo renderPassBeginInfo;
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.framebuffer = framebuffers[i];
    renderPassBeginInfo.renderArea.offset = {0, 0};
    renderPassBeginInfo.renderArea.extent = {window_width, window_height};

		VkClearValue clearValue = {0.f, 0.f, 0.f, 1.f};
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &clearValue;

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
		vkCmdEndRenderPass(commandBuffers[i]);

		error << vkEndCommandBuffer(commandBuffers[i]);
	}

	VkSemaphoreCreateInfo semaphoreInfo;
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  semaphoreInfo.pNext = nullptr;
  semaphoreInfo.flags = 0;

	VkSemaphore semaphoreImageAvailable;
	error << vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphoreImageAvailable);
	VkSemaphore semaphoreRenderingDone;;
	error << vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphoreRenderingDone);

	VkQueue queue;
	vkGetDeviceQueue(device, 0, 0, &queue);

	// we initialzed vulkan (yay)
	std::cout << "hello vulkan" << std::endl;

	std::uint32_t imageIndex;
	error << vkAcquireNextImageKHR(device, swapchain, std::numeric_limits<std::uint64_t>::max(), semaphoreImageAvailable, VK_NULL_HANDLE, &imageIndex);
	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &semaphoreImageAvailable;
	VkPipelineStageFlags waitStages[] {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &semaphoreRenderingDone;

	error << vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

	VkPresentInfoKHR presentInfo;
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.pNext = nullptr;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = &semaphoreRenderingDone;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &swapchain;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = nullptr;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_Q))
			glfwSetWindowShouldClose(window, true);
	}

	// destroy vulkan
	vkDeviceWaitIdle(device);
	vkDestroySemaphore(device, semaphoreRenderingDone, nullptr);
	vkDestroySemaphore(device, semaphoreImageAvailable, nullptr);
	vkFreeCommandBuffers(device, commandPool, imageCount, commandBuffers.data()); // implicit when pool gets destroyed
	vkDestroyCommandPool(device, commandPool, nullptr);
	for (const auto& framebuffer : framebuffers)
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyShaderModule(device, shaderVert, nullptr);
	vkDestroyShaderModule(device, shaderFrag, nullptr);
	for (const auto& imageView : imageViews)
		vkDestroyImageView(device, imageView, nullptr);
	vkDestroySwapchainKHR(device, swapchain, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
