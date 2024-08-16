#include <vulkan/vulkan.h>

#include "../../utils/debugprint.h"
#include <cstdint>

uint32_t findMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags properties, const VkPhysicalDevice physicalDevice) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    // https://vulkan-tutorial.com/Vertex_buffers/Vertex_buffer_creation
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    debugerror("failed to find suitable memory type!");
}
