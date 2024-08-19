#pragma once
#include <vulkan/vulkan.h>

#include "../enginedata.h"
#include <optional>
#include <vector>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    std::optional<uint32_t> transferFamily;

    bool isComplete();
    bool isSame();
};

QueueFamilyIndices      findQueueFamilies(const VkPhysicalDevice, const VkSurfaceKHR&);
VkExtent2D              chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capiblilies, const WindowResources& window);
SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);
VkSurfaceFormatKHR      chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR        chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes);
VkExtent2D              chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capiblilies, const WindowResources& window);
void                    pickPhysicalDevice(const VkInstance                instance,
                                           VkPhysicalDevice&               physicalDevice,
                                           const VkSurfaceKHR              surface,
                                           const std::vector<const char*>& deviceExtensions);
