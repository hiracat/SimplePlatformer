#pragma once
#include <vulkan/vulkan.h>

#include "../enginedata.h"
#include <vector>

QueueFamilyIndices      findQueueFamilies(const VkPhysicalDevice, const VkSurfaceKHR&);
VkExtent2D              chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capiblilies, const WindowResources& window);
SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);
VkSurfaceFormatKHR      chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR        chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes);
VkExtent2D              chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capiblilies, const WindowResources& window);
void                    pickPhysicalDevice(const VkInstance                instance,
                                           const std::vector<const char*>& deviceExtensions,
                                           VkPhysicalDevice&               physicalDevice,
                                           const VkSurfaceKHR              surface);
