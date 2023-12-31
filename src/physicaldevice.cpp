#include <cstdint>
#include <map>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

#include <vulkan/vulkan_core.h>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() {
        return graphicsFamily.has_value();
    }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount{};
    vkGetPhysicalDeviceQueueFamilyProperties2(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties2> queueFamilies(queueFamilyCount);
    for (auto& queueFamily : queueFamilies) {
        queueFamily.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
        queueFamily.pNext = nullptr;
    }
    vkGetPhysicalDeviceQueueFamilyProperties2(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
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

uint32_t scorePhysicalDevice(const VkPhysicalDevice& device) {
    VkPhysicalDeviceProperties2 deviceProperties{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, .pNext = nullptr};
    VkPhysicalDeviceFeatures2   deviceFeatures{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, .pNext = nullptr};
    vkGetPhysicalDeviceProperties2(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures2(device, &deviceFeatures);

    QueueFamilyIndices indices = findQueueFamilies(device);

    return indices.isComplete();
}

void pickPhysicalDevice(VkInstance instance, VkPhysicalDevice physicalDevice) {
    uint32_t physicalDeviceCount{};

    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
    if (physicalDeviceCount < 1) {
        throw std::runtime_error("no devices with vulkan support available");
    }

    std::vector<VkPhysicalDevice> devices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, devices.data());

    std::multimap<uint32_t, VkPhysicalDevice> deviceScores;
    for (const auto& device : devices) {
        deviceScores.insert(std::make_pair(scorePhysicalDevice(device), device));
    }
    if (deviceScores.rbegin()->first > 0) {
        physicalDevice = deviceScores.rbegin()->second;
    } else {
        throw std::runtime_error("no good enough gpus available");
    }
}
