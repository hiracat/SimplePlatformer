#include <cstdint>
#include <vulkan/vulkan_core.h>

uint32_t findMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags properties, const VkPhysicalDevice physicalDevice);
