#include <cstdint>
#include <iostream>
#include <map>
#include <optional>
#include <ostream>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "ansiescapecodes.h"
#include "physicaldevice.h"

bool QueueFamilyIndices::isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
}

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount{};
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    if (formatCount) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModesCount{};
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModesCount, nullptr);
    if (presentModesCount) {
        details.presentModes.resize(presentModesCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModesCount, details.presentModes.data());
    }
    return details;
}

QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount{};
    vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties2> queueFamilies(queueFamilyCount);

    for (auto& queueFamily : queueFamilies) {
        queueFamily.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
        queueFamily.pNext = nullptr;
    }
    vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }
        if (queueFamily.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        if (indices.isComplete()) {
            break;
        }
        i++;
    }

    return indices;
}

bool checkDeviceExtensionSupport(VkPhysicalDevice device, std::vector<const char*> deviceExtensions) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    for (const auto& extensions : availableExtensions) {
        requiredExtensions.erase(extensions.extensionName);
    }
    return requiredExtensions.empty();
}

uint32_t scorePhysicalDevice(const VkPhysicalDevice&         device,
                             const VkSurfaceKHR&             surface,
                             const std::vector<const char*>& deviceExtensions) {
    uint32_t                    score = 1;
    VkPhysicalDeviceProperties2 deviceProperties{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, .pNext = nullptr};
    VkPhysicalDeviceFeatures2   deviceFeatures{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, .pNext = nullptr};
    vkGetPhysicalDeviceProperties2(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures2(device, &deviceFeatures);

    // keep this in the front so that function calls that need an extension from here dont crash it
    if (!checkDeviceExtensionSupport(device, deviceExtensions)) {
        return 0;
    }
    QueueFamilyIndices indices = findQueueFamilies(device, surface);
    if (indices.presentFamily != indices.graphicsFamily) {
        score += 50;
    }
    if (!indices.isComplete()) {
        return 0;
    }

    SwapChainSupportDetails swapChainSupport  = querySwapChainSupport(device, surface);
    bool                    swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    if (!swapChainAdequate) {
        return 0;
    }

    return score;
}

void pickPhysicalDevice(VkInstance                      instance,
                        VkPhysicalDevice&               physicalDevice,
                        const VkSurfaceKHR&             surface,
                        const std::vector<const char*>& deviceExtensions) {
    uint32_t physicalDeviceCount{};

    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
    if (physicalDeviceCount < 1) {
        throw std::runtime_error("no devices with vulkan support available");
    }

    std::vector<VkPhysicalDevice> devices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, devices.data());

    std::multimap<uint32_t, VkPhysicalDevice> deviceScores;
    for (const auto& device : devices) {
        deviceScores.insert(std::make_pair(scorePhysicalDevice(device, surface, deviceExtensions), device));
    }
    if (deviceScores.rbegin()->first > 0) {
        physicalDevice = deviceScores.rbegin()->second;
    } else {
        throw std::runtime_error("no good enough gpus available");
    }
}
