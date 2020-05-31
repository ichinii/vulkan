#include "log.h"
#include <iostream>
#include <vector>

using namespace std::string_literals;

namespace debug {

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

void dump(const VkPhysicalDeviceLimits& limits)
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

void dump(const VkPhysicalDeviceFeatures& features)
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

void dump(const VkPhysicalDevice& device)
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
		dump(properties.limits);
		// std::cout << "\tsparse properties: " << "..." << std::endl; // properties.sparseProperties
	
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(device, &features);
	dump(features);

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

void dump(const VkLayerProperties &layer)
{
	std::cout << "layer" << std::endl
		<< "\tname:                   " << layer.layerName << std::endl
		<< "\tspecification version:  " << layer.specVersion << std::endl
		<< "\timplementation version: " << layer.implementationVersion << std::endl
		<< "\tdescription:            " << layer.description << std::endl;
}

void dump(const VkExtensionProperties &extension)
{
	std::cout << "extension" << std::endl
		<< "\tname:                  " << extension.extensionName << std::endl
		<< "\tspecification version: " << extension.specVersion << std::endl;
}

void dump(const VkSurfaceCapabilitiesKHR& capabilities)
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

void dump(const VkSurfaceFormatKHR& format)
{
	std::cout << "\t" << format.format << " (" << format_string[format.format] << "), "
		<< format.colorSpace << " (" << color_space_string[format.colorSpace] << ")"<< std::endl;
}

void dump(const VkPresentModeKHR& mode)
{
		std::cout << "\t" << mode << " (" << present_mode_string[mode] << ")" << std::endl;
}

};
