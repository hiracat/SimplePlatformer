#pragma once
#include <vulkan/vulkan.h>

#include "../engine.h"
#include <vector>

// set physicaldeviceoverride to chose a custom physical device, otherwise it is just the one in vulkandata
void       findQueueFamilies(const VulkanData& vulkanData, const VkPhysicalDevice physicalDeviceOverride, QueueFamilyIndices* indices);
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capiblilies, const WindowData& window);
SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);
VkSurfaceFormatKHR      chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR        chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes);
VkExtent2D              chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capiblilies, const WindowData& window);
void                    pickPhysicalDevice(const VulkanData& vulkanData, VkPhysicalDevice* physicalDevice);
