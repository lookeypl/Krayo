#pragma once

#include <vulkan/vulkan.h>

#include <string>


namespace Krayo {
namespace Renderer {

uint32_t TranslateVkFormatToFormatSize(VkFormat format);
const char* TranslateVkFormatToString(VkFormat format);
const char* TranslateVkPhysicalDeviceTypeToString(VkPhysicalDeviceType type);
const char* TranslateVkQueueFlagsToString(VkQueueFlags flags);
const char* TranslateVkResultToString(VkResult result);

} // namespace Renderer
} // namespace Krayo
