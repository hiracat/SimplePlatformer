#pragma once

#include <optional>

#include <vulkan/vulkan_core.h>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete();
};

void pickPhysicalDevice(VkInstance, VkPhysicalDevice&, const VkSurfaceKHR&);

QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice&, const VkSurfaceKHR&);
