#include <vulkan/vulkan.h>

#include <cstdint>

uint32_t findMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags properties, const VkPhysicalDevice physicalDevice);
