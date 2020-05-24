#include <bits/stdc++.h>
#include <vulkan/vulkan.h>
// #include <vulkan/vulkan_intel.h>

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

std::unordered_map<std::underlying_type_t<VkResult>, std::string> physical_device_type_name {
	{VK_PHYSICAL_DEVICE_TYPE_OTHER,          "VK_PHYSICAL_DEVICE_TYPE_OTHER"},
	{VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU, "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU"},
	{VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,   "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU"},
	{VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,    "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU"},
	{VK_PHYSICAL_DEVICE_TYPE_CPU,            "VK_PHYSICAL_DEVICE_TYPE_CPU"},
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
		<< "\t\tmaxImageDimension1D: "                             << limits.maxImageDimension1D << std::endl
		<< "\t\tmaxImageDimension2D: "                             << limits.maxImageDimension2D << std::endl
		<< "\t\tmaxImageDimension3D: "                             << limits.maxImageDimension3D << std::endl
		<< "\t\tmaxImageDimensionCube: "                           << limits.maxImageDimensionCube << std::endl
		<< "\t\tmaxImageArrayLayers: "                             << limits.maxImageArrayLayers << std::endl
		<< "\t\tmaxTexelBufferElements: "                          << limits.maxTexelBufferElements << std::endl
		<< "\t\tmaxUniformBufferRange: "                           << limits.maxUniformBufferRange << std::endl
		<< "\t\tmaxStorageBufferRange: "                           << limits.maxStorageBufferRange << std::endl
		<< "\t\tmaxPushConstantsSize: "                            << limits.maxPushConstantsSize << std::endl
		<< "\t\tmaxMemoryAllocationCount: "                        << limits.maxMemoryAllocationCount << std::endl
		<< "\t\tmaxSamplerAllocationCount: "                       << limits.maxSamplerAllocationCount << std::endl
		<< "\t\tbufferImageGranularity: "                          << limits.bufferImageGranularity << std::endl
		<< "\t\tsparseAddressSpaceSize: "                          << limits.sparseAddressSpaceSize << std::endl
		<< "\t\tmaxBoundDescriptorSets: "                          << limits.maxBoundDescriptorSets << std::endl
		<< "\t\tmaxPerStageDescriptorSamplers: "                   << limits.maxPerStageDescriptorSamplers << std::endl
		<< "\t\tmaxPerStageDescriptorUniformBuffers: "             << limits.maxPerStageDescriptorUniformBuffers << std::endl
		<< "\t\tmaxPerStageDescriptorStorageBuffers: "             << limits.maxPerStageDescriptorStorageBuffers << std::endl
		<< "\t\tmaxPerStageDescriptorSampledImages: "              << limits.maxPerStageDescriptorSampledImages << std::endl
		<< "\t\tmaxPerStageDescriptorStorageImages: "              << limits.maxPerStageDescriptorStorageImages << std::endl
		<< "\t\tmaxPerStageDescriptorInputAttachments: "           << limits.maxPerStageDescriptorInputAttachments << std::endl
		<< "\t\tmaxPerStageResources: "                            << limits.maxPerStageResources << std::endl
		<< "\t\tmaxDescriptorSetSamplers: "                        << limits.maxDescriptorSetSamplers << std::endl
		<< "\t\tmaxDescriptorSetUniformBuffers: "                  << limits.maxDescriptorSetUniformBuffers << std::endl
		<< "\t\tmaxDescriptorSetUniformBuffersDynamic: "           << limits.maxDescriptorSetUniformBuffersDynamic << std::endl
		<< "\t\tmaxDescriptorSetStorageBuffers: "                  << limits.maxDescriptorSetStorageBuffers << std::endl
		<< "\t\tmaxDescriptorSetStorageBuffersDynamic: "           << limits.maxDescriptorSetStorageBuffersDynamic << std::endl
		<< "\t\tmaxDescriptorSetSampledImages: "                   << limits.maxDescriptorSetSampledImages << std::endl
		<< "\t\tmaxDescriptorSetStorageImages: "                   << limits.maxDescriptorSetStorageImages << std::endl
		<< "\t\tmaxDescriptorSetInputAttachments: "                << limits.maxDescriptorSetInputAttachments << std::endl
		<< "\t\tmaxVertexInputAttributes: "                        << limits.maxVertexInputAttributes << std::endl
		<< "\t\tmaxVertexInputBindings: "                          << limits.maxVertexInputBindings << std::endl
		<< "\t\tmaxVertexInputAttributeOffset: "                   << limits.maxVertexInputAttributeOffset << std::endl
		<< "\t\tmaxVertexInputBindingStride: "                     << limits.maxVertexInputBindingStride << std::endl
		<< "\t\tmaxVertexOutputComponents: "                       << limits.maxVertexOutputComponents << std::endl
		<< "\t\tmaxTessellationGenerationLevel: "                  << limits.maxTessellationGenerationLevel << std::endl
		<< "\t\tmaxTessellationPatchSize: "                        << limits.maxTessellationPatchSize << std::endl
		<< "\t\tmaxTessellationControlPerVertexInputComponents: "  << limits.maxTessellationControlPerVertexInputComponents << std::endl
		<< "\t\tmaxTessellationControlPerVertexOutputComponents: " << limits.maxTessellationControlPerVertexOutputComponents << std::endl
		<< "\t\tmaxTessellationControlPerPatchOutputComponents: "  << limits.maxTessellationControlPerPatchOutputComponents << std::endl
		<< "\t\tmaxTessellationControlTotalOutputComponents: "     << limits.maxTessellationControlTotalOutputComponents << std::endl
		<< "\t\tmaxTessellationEvaluationInputComponents: "        << limits.maxTessellationEvaluationInputComponents << std::endl
		<< "\t\tmaxTessellationEvaluationOutputComponents: "       << limits.maxTessellationEvaluationOutputComponents << std::endl
		<< "\t\tmaxGeometryShaderInvocations: "                    << limits.maxGeometryShaderInvocations << std::endl
		<< "\t\tmaxGeometryInputComponents: "                      << limits.maxGeometryInputComponents << std::endl
		<< "\t\tmaxGeometryOutputComponents: "                     << limits.maxGeometryOutputComponents << std::endl
		<< "\t\tmaxGeometryOutputVertices: "                       << limits.maxGeometryOutputVertices << std::endl
		<< "\t\tmaxGeometryTotalOutputComponents: "                << limits.maxGeometryTotalOutputComponents << std::endl
		<< "\t\tmaxFragmentInputComponents: "                      << limits.maxFragmentInputComponents << std::endl
		<< "\t\tmaxFragmentOutputAttachments: "                    << limits.maxFragmentOutputAttachments << std::endl
		<< "\t\tmaxFragmentDualSrcAttachments: "                   << limits.maxFragmentDualSrcAttachments << std::endl
		<< "\t\tmaxFragmentCombinedOutputResources: "              << limits.maxFragmentCombinedOutputResources << std::endl
		<< "\t\tmaxComputeSharedMemorySize: "                      << limits.maxComputeSharedMemorySize << std::endl
		<< "\t\tmaxComputeWorkGroupCount[3]: "
			<< limits.maxComputeWorkGroupCount[0] << ", "
			<< limits.maxComputeWorkGroupCount[1] << ", "
			<< limits.maxComputeWorkGroupCount[2] << std::endl
		<< "\t\tmaxComputeWorkGroupInvocations: "                  << limits.maxComputeWorkGroupInvocations << std::endl
		<< "\t\tmaxComputeWorkGroupSize[3]: "
			<< limits.maxComputeWorkGroupSize[0] << ", "
			<< limits.maxComputeWorkGroupSize[1] << ", "
			<< limits.maxComputeWorkGroupSize[2] << std::endl
		<< "\t\tsubPixelPrecisionBits: "                           << limits.subPixelPrecisionBits << std::endl
		<< "\t\tsubTexelPrecisionBits: "                           << limits.subTexelPrecisionBits << std::endl
		<< "\t\tmipmapPrecisionBits: "                             << limits.mipmapPrecisionBits << std::endl
		<< "\t\tmaxDrawIndexedIndexValue: "                        << limits.maxDrawIndexedIndexValue << std::endl
		<< "\t\tmaxDrawIndirectCount: "                            << limits.maxDrawIndirectCount << std::endl
		<< "\t\tmaxSamplerLodBias: "                               << limits.maxSamplerLodBias << std::endl
		<< "\t\tmaxSamplerAnisotropy: "                            << limits.maxSamplerAnisotropy << std::endl
		<< "\t\tmaxViewports: "                                    << limits.maxViewports << std::endl
		<< "\t\tmaxViewportDimensions[2]: "
			<< limits.maxViewportDimensions[0] << ", "
			<< limits.maxViewportDimensions[1] << std::endl
		<< "\t\tviewportBoundsRange[2]: "
			<< limits.viewportBoundsRange[0] << ", "
			<< limits.viewportBoundsRange[1] << std::endl
		<< "\t\tviewportSubPixelBits: "                            << limits.viewportSubPixelBits << std::endl
		<< "\t\tminMemoryMapAlignment: "                           << limits.minMemoryMapAlignment << std::endl
		<< "\t\tminTexelBufferOffsetAlignment: "                   << limits.minTexelBufferOffsetAlignment << std::endl
		<< "\t\tminUniformBufferOffsetAlignment: "                 << limits.minUniformBufferOffsetAlignment << std::endl
		<< "\t\tminStorageBufferOffsetAlignment: "                 << limits.minStorageBufferOffsetAlignment << std::endl
		<< "\t\tminTexelOffset: "                                  << limits.minTexelOffset << std::endl
		<< "\t\tmaxTexelOffset: "                                  << limits.maxTexelOffset << std::endl
		<< "\t\tminTexelGatherOffset: "                            << limits.minTexelGatherOffset << std::endl
		<< "\t\tmaxTexelGatherOffset: "                            << limits.maxTexelGatherOffset << std::endl
		<< "\t\tminInterpolationOffset: "                          << limits.minInterpolationOffset << std::endl
		<< "\t\tmaxInterpolationOffset: "                          << limits.maxInterpolationOffset << std::endl
		<< "\t\tsubPixelInterpolationOffsetBits: "                 << limits.subPixelInterpolationOffsetBits << std::endl
		<< "\t\tmaxFramebufferWidth: "                             << limits.maxFramebufferWidth << std::endl
		<< "\t\tmaxFramebufferHeight: "                            << limits.maxFramebufferHeight << std::endl
		<< "\t\tmaxFramebufferLayers: "                            << limits.maxFramebufferLayers << std::endl
		<< "\t\tframebufferColorSampleCounts: "                    << limits.framebufferColorSampleCounts << std::endl
		<< "\t\tframebufferDepthSampleCounts: "                    << limits.framebufferDepthSampleCounts << std::endl
		<< "\t\tframebufferStencilSampleCounts: "                  << limits.framebufferStencilSampleCounts << std::endl
		<< "\t\tframebufferNoAttachmentsSampleCounts: "            << limits.framebufferNoAttachmentsSampleCounts << std::endl
		<< "\t\tmaxColorAttachments: "                             << limits.maxColorAttachments << std::endl
		<< "\t\tsampledImageColorSampleCounts: "                   << limits.sampledImageColorSampleCounts << std::endl
		<< "\t\tsampledImageIntegerSampleCounts: "                 << limits.sampledImageIntegerSampleCounts << std::endl
		<< "\t\tsampledImageDepthSampleCounts: "                   << limits.sampledImageDepthSampleCounts << std::endl
		<< "\t\tsampledImageStencilSampleCounts: "                 << limits.sampledImageStencilSampleCounts << std::endl
		<< "\t\tstorageImageSampleCounts: "                        << limits.storageImageSampleCounts << std::endl
		<< "\t\tmaxSampleMaskWords: "                              << limits.maxSampleMaskWords << std::endl
		<< "\t\ttimestampComputeAndGraphics: "                     << limits.timestampComputeAndGraphics << std::endl
		<< "\t\ttimestampPeriod: "                                 << limits.timestampPeriod << std::endl
		<< "\t\tmaxClipDistances: "                                << limits.maxClipDistances << std::endl
		<< "\t\tmaxCullDistances: "                                << limits.maxCullDistances << std::endl
		<< "\t\tmaxCombinedClipAndCullDistances: "                 << limits.maxCombinedClipAndCullDistances << std::endl
		<< "\t\tdiscreteQueuePriorities: "                         << limits.discreteQueuePriorities << std::endl
		<< "\t\tpointSizeRange[2]: "
			<< limits.pointSizeRange[0] << ", "
			<< limits.pointSizeRange[1] << std::endl
		<< "\t\tlineWidthRange[2]: "
			<< limits.lineWidthRange[0] << ", "
			<< limits.lineWidthRange[0] << std::endl
		<< "\t\tpointSizeGranularity: "                            << limits.pointSizeGranularity << std::endl
		<< "\t\tlineWidthGranularity: "                            << limits.lineWidthGranularity << std::endl
		<< "\t\tstrictLines: "                                     << limits.strictLines << std::endl
		<< "\t\tstandardSampleLocations: "                         << limits.standardSampleLocations << std::endl
		<< "\t\toptimalBufferCopyOffsetAlignment: "                << limits.optimalBufferCopyOffsetAlignment << std::endl
		<< "\t\toptimalBufferCopyRowPitchAlignment: "              << limits.optimalBufferCopyRowPitchAlignment << std::endl
		<< "\t\tnonCoherentAtomSize: "                             << limits.nonCoherentAtomSize << std::endl;
}

void dumpPhysicalDeviceFeatures(const VkPhysicalDeviceFeatures& features)
{
	std::cout << "\tfeatures" << std::endl
		<< "\t\trobustBufferAccess: "                      << features.robustBufferAccess << std::endl
		<< "\t\tfullDrawIndexUint32: "                     << features.fullDrawIndexUint32 << std::endl
		<< "\t\timageCubeArray: "                          << features.imageCubeArray << std::endl
		<< "\t\tindependentBlend: "                        << features.independentBlend << std::endl
		<< "\t\tgeometryShader: "                          << features.geometryShader << std::endl
		<< "\t\ttessellationShader: "                      << features.tessellationShader << std::endl
		<< "\t\tsampleRateShading: "                       << features.sampleRateShading << std::endl
		<< "\t\tdualSrcBlend: "                            << features.dualSrcBlend << std::endl
		<< "\t\tlogicOp: "                                 << features.logicOp << std::endl
		<< "\t\tmultiDrawIndirect: "                       << features.multiDrawIndirect << std::endl
		<< "\t\tdrawIndirectFirstInstance: "               << features.drawIndirectFirstInstance << std::endl
		<< "\t\tdepthClamp: "                              << features.depthClamp << std::endl
		<< "\t\tdepthBiasClamp: "                          << features.depthBiasClamp << std::endl
		<< "\t\tfillModeNonSolid: "                        << features.fillModeNonSolid << std::endl
		<< "\t\tdepthBounds: "                             << features.depthBounds << std::endl
		<< "\t\twideLines: "                               << features.wideLines << std::endl
		<< "\t\tlargePoints: "                             << features.largePoints << std::endl
		<< "\t\talphaToOne: "                              << features.alphaToOne << std::endl
		<< "\t\tmultiViewport: "                           << features.multiViewport << std::endl
		<< "\t\tsamplerAnisotropy: "                       << features.samplerAnisotropy << std::endl
		<< "\t\ttextureCompressionETC2: "                  << features.textureCompressionETC2 << std::endl
		<< "\t\ttextureCompressionASTC_LDR: "              << features.textureCompressionASTC_LDR << std::endl
		<< "\t\ttextureCompressionBC: "                    << features.textureCompressionBC << std::endl
		<< "\t\tocclusionQueryPrecise: "                   << features.occlusionQueryPrecise << std::endl
		<< "\t\tpipelineStatisticsQuery: "                 << features.pipelineStatisticsQuery << std::endl
		<< "\t\tvertexPipelineStoresAndAtomics: "          << features.vertexPipelineStoresAndAtomics << std::endl
		<< "\t\tfragmentStoresAndAtomics: "                << features.fragmentStoresAndAtomics << std::endl
		<< "\t\tshaderTessellationAndGeometryPointSize: "  << features.shaderTessellationAndGeometryPointSize << std::endl
		<< "\t\tshaderImageGatherExtended: "               << features.shaderImageGatherExtended << std::endl
		<< "\t\tshaderStorageImageExtendedFormats: "       << features.shaderStorageImageExtendedFormats << std::endl
		<< "\t\tshaderStorageImageMultisample: "           << features.shaderStorageImageMultisample << std::endl
		<< "\t\tshaderStorageImageReadWithoutFormat: "     << features.shaderStorageImageReadWithoutFormat << std::endl
		<< "\t\tshaderStorageImageWriteWithoutFormat: "    << features.shaderStorageImageWriteWithoutFormat << std::endl
		<< "\t\tshaderUniformBufferArrayDynamicIndexing: " << features.shaderUniformBufferArrayDynamicIndexing << std::endl
		<< "\t\tshaderSampledImageArrayDynamicIndexing: "  << features.shaderSampledImageArrayDynamicIndexing << std::endl
		<< "\t\tshaderStorageBufferArrayDynamicIndexing: " << features.shaderStorageBufferArrayDynamicIndexing << std::endl
		<< "\t\tshaderStorageImageArrayDynamicIndexing: "  << features.shaderStorageImageArrayDynamicIndexing << std::endl
		<< "\t\tshaderClipDistance: "                      << features.shaderClipDistance << std::endl
		<< "\t\tshaderCullDistance: "                      << features.shaderCullDistance << std::endl
		<< "\t\tshaderFloat64: "                           << features.shaderFloat64 << std::endl
		<< "\t\tshaderInt64: "                             << features.shaderInt64 << std::endl
		<< "\t\tshaderInt16: "                             << features.shaderInt16 << std::endl
		<< "\t\tshaderResourceResidency: "                 << features.shaderResourceResidency << std::endl
		<< "\t\tshaderResourceMinLod: "                    << features.shaderResourceMinLod << std::endl
		<< "\t\tsparseBinding: "                           << features.sparseBinding << std::endl
		<< "\t\tsparseResidencyBuffer: "                   << features.sparseResidencyBuffer << std::endl
		<< "\t\tsparseResidencyImage2D: "                  << features.sparseResidencyImage2D << std::endl
		<< "\t\tsparseResidencyImage3D: "                  << features.sparseResidencyImage3D << std::endl
		<< "\t\tsparseResidency2Samples: "                 << features.sparseResidency2Samples << std::endl
		<< "\t\tsparseResidency4Samples: "                 << features.sparseResidency4Samples << std::endl
		<< "\t\tsparseResidency8Samples: "                 << features.sparseResidency8Samples << std::endl
		<< "\t\tsparseResidency16Samples: "                << features.sparseResidency16Samples << std::endl
		<< "\t\tsparseResidencyAliased: "                  << features.sparseResidencyAliased << std::endl
		<< "\t\tvariableMultisampleRate: "                 << features.variableMultisampleRate << std::endl
		<< "\t\tinheritedQueries: "                        << features.inheritedQueries << std::endl;
}

void dumpPhysicalDevice(const VkPhysicalDevice& device)
{
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(device, &properties);
	std::cout << "device properties" << std::endl
		<< "\tapi version: "
			<< VK_VERSION_MAJOR(properties.apiVersion) << "."
			<< VK_VERSION_MINOR(properties.apiVersion) << "."
			<< VK_VERSION_PATCH(properties.apiVersion) << std::endl
		<< "\tdiver version: " << properties.driverVersion << std::endl
		<< "\tvendor id: " << properties.vendorID << std::endl
		<< "\tdevice id: " << properties.deviceID << std::endl
		<< "\tdevice type: " << properties.deviceType << " (" << physical_device_type_name[properties.deviceType] << ")" << std::endl
		<< "\tdevice name: " << properties.deviceName << std::endl
		<< "\tpipeline cach uuid: " << properties.pipelineCacheUUID << std::endl;
		dumpPhysicalDeviceLimit(properties.limits);
		// std::cout << "\tsparse properties: " << "..." << std::endl; // properties.sparseProperties
	
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(device, &features);
	dumpPhysicalDeviceFeatures(features);

	// VkPhysicalDeviceMemoryProperties memory;
	// vkGetPhysicalDeviceMemoryProperties(device, &memory);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	VkApplicationInfo appInfo;
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pNext = nullptr;
  appInfo.pApplicationName = "Generic Sauce Vulkan Application";
  appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
  appInfo.pEngineName = "Generic Sauce Vulkan Graphics Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
  appInfo.apiVersion = VK_VERSION_1_0;

	VkInstanceCreateInfo instanceInfo;
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext = nullptr;
	instanceInfo.flags = 0;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledLayerCount = 0;
	instanceInfo.ppEnabledLayerNames = nullptr;
	instanceInfo.enabledExtensionCount = 0;
	instanceInfo.ppEnabledExtensionNames = nullptr;

	VkInstance instance;
	error << vkCreateInstance(&instanceInfo, nullptr, &instance);

	std::uint32_t physicalDeviceCount;
	error << vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
	auto physicalDevices = std::vector<VkPhysicalDevice>(physicalDeviceCount);
	error << vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

	for (const auto& device : physicalDevices)
		dumpPhysicalDevice(device);

	std::cout << "hello vulkan" << std::endl;
	return 0;
}