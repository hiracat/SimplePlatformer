#pragma once

#include <optional>

#include <vector>
#include <vulkan/vulkan_core.h>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete();
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentModes;
};

void pickPhysicalDevice(VkInstance, VkPhysicalDevice&, const VkSurfaceKHR&, const std::vector<const char*>&);

QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice&, const VkSurfaceKHR&);
