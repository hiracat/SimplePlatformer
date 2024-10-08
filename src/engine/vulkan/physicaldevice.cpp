#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include "../engine.h"
#include <algorithm>
#include <cstdint>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "../../utils/debugprint.h"
#include "physicaldevice.h"

bool QueueFamilyIndices::isComplete() const {
    return graphics.has_value() && present.has_value() && transfer.has_value();
}
bool QueueFamilyIndices::isSame() const {
    return (graphics.value() == present.value()) && (transfer.value() == graphics.value());
}

void findQueueFamilies(const VulkanData& vulkanData, const VkPhysicalDevice physicalDeviceOverride, QueueFamilyIndices* indices) {
    VkPhysicalDevice physicalDevice;

    if (physicalDeviceOverride != VK_NULL_HANDLE) {
        physicalDevice = physicalDeviceOverride;
    } else {
        physicalDevice = vulkanData.physicalDevice;
    }
    uint32_t queueFamilyCount{};

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

            indices->transfer = currentQueueIndex;
        }

        // Check for a dedicated graphics queue
        if ((queueFamily.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
            indices->graphics = currentQueueIndex;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, currentQueueIndex, vulkanData.surface, &presentSupport);
        if (presentSupport && !indices->present.has_value()) {
            indices->present = currentQueueIndex;
        }
        currentQueueIndex++;
    }

    if (!indices->transfer.has_value()) {
        indices->transfer = indices->graphics.value();
    }

    if (!indices->isComplete()) {
        debugerror("no queue with present support");
    }
    debugnote("graphics queue: " << indices->graphics.value());
    debugnote("transfer queue: " << indices->transfer.value());
    debugnote("present queue: " << indices->present.value());
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
                             QueueFamilyIndices              indices,
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

void pickPhysicalDevice(const VulkanData& vulkanData, VkPhysicalDevice* physicalDevice) {
    uint32_t physicalDeviceCount{};

    vkEnumeratePhysicalDevices(vulkanData.instance, &physicalDeviceCount, nullptr);
    if (physicalDeviceCount < 1) {
        debugerror("no devices with vulkan support available");
    }

    std::vector<VkPhysicalDevice> devices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(vulkanData.instance, &physicalDeviceCount, devices.data());

    std::multimap<uint32_t, VkPhysicalDevice> deviceScores;

    QueueFamilyIndices currentDeviceIndices;

    for (const auto& device : devices) {
        findQueueFamilies(vulkanData, device, &currentDeviceIndices);
        deviceScores.insert(
            std::make_pair(scorePhysicalDevice(device, currentDeviceIndices, vulkanData.surface, vulkanData.deviceExtensions), device));
    }
    if (deviceScores.rbegin()->first > 0) {
        *physicalDevice = deviceScores.rbegin()->second;
    } else {
        debugerror("no good enough gpus available");
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

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capiblilies, const WindowData& window) {
    if (capiblilies.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capiblilies.currentExtent;
    } else { // special case, where widow size does not have to match swapchain resolution in certian window managers
        int width, height;
        glfwGetFramebufferSize(window.window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height),
        };
        actualExtent.width  = std::clamp(actualExtent.width, capiblilies.minImageExtent.width, capiblilies.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capiblilies.minImageExtent.height, capiblilies.maxImageExtent.height);
        return actualExtent;
    }
}
