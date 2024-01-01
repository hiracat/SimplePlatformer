#pragma once

#include <optional>

#include <vulkan/vulkan_core.h>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete();
};

void pickPhysicalDevice(VkInstance, VkPhysicalDevice&);

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
