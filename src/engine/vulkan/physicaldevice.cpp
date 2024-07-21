#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include <algorithm>
#include <cstdint>
#include <limits>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "../../utils/debugprint.h"
#include "../window.h"
#include "physicaldevice.h"

bool QueueFamilyIndices::isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();
}

// requires vksurface to determine if a queue family supports presentation to a given surface
QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR& surface) {
    QueueFamilyIndices indices;
    uint32_t           queueFamilyCount{};

    // NOTE: getting queuefamily properties
    vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties2> queueFamilies(queueFamilyCount);
    for (auto& queueFamily : queueFamilies) {
        queueFamily.sType                 = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
        queueFamily.pNext                 = nullptr;
        queueFamily.queueFamilyProperties = {0};
    }
    vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &queueFamilyCount, queueFamilies.data());

    // NOTE: finding the best combination of queues
    int currentQueueIndex = 0;
    // check for dedicated queues
    for (const auto& queueFamily : queueFamilies) {

        if ((queueFamily.queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT) &&
            !(queueFamily.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
            !(queueFamily.queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT)) {

            indices.transferFamily = currentQueueIndex;
        }

        // Check for a dedicated graphics queue
        if ((queueFamily.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
            indices.graphicsFamily = currentQueueIndex;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, currentQueueIndex, surface, &presentSupport);
        if (presentSupport && !indices.presentFamily.has_value()) {
            indices.presentFamily = currentQueueIndex;
        }
        currentQueueIndex++;
    }

    if (!indices.transferFamily.has_value()) {
        indices.transferFamily = indices.graphicsFamily.value();
    }

    if (!indices.isComplete()) {
        throw std::runtime_error("no queue with present support");
    }
    debugnote("graphics queue: " << indices.graphicsFamily.value());
    debugnote("transfer queue: " << indices.transferFamily.value());
    debugnote("present queue: " << indices.presentFamily.value());
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

uint32_t
scorePhysicalDevice(const VkPhysicalDevice& device, const VkSurfaceKHR& surface, const std::vector<const char*>& deviceExtensions) {
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

void pickPhysicalDevice(const VkInstance                instance,
                        VkPhysicalDevice&               physicalDevice,
                        const VkSurfaceKHR              surface,
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

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_R8G8B8A8_SRGB && availableFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes) {
    for (const auto& presentMode : presentModes) {
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return presentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR; // this is the only present mode that is guaranteed
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capiblilies, const Window& window) {
    if (capiblilies.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capiblilies.currentExtent;
    } else { // special case, where widow size does not have to match swapchain resolution in certian window managers
        int width, height;
        glfwGetFramebufferSize(window.windowPointer, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height),
        };
        actualExtent.width  = std::clamp(actualExtent.width, capiblilies.minImageExtent.width, capiblilies.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capiblilies.minImageExtent.height, capiblilies.maxImageExtent.height);
        return actualExtent;
    }
}
