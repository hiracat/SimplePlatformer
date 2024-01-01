#include <cstdint>
#include <iostream>
#include <map>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "ansiescapecodes.h"
#include "physicaldevice.h"

bool QueueFamilyIndices::isComplete() {
    return graphicsFamily.has_value();
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice) {
    std::cout << "how the fuck did i get into find queue families" << std::endl;
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount{};
    vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &queueFamilyCount, nullptr);
    std::cout << "if you see this im not dying of vkGet physicalDeviceProperties" << std::endl;

    std::vector<VkQueueFamilyProperties2> queueFamilies(queueFamilyCount);
    for (auto& queueFamily : queueFamilies) {
        queueFamily.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
        queueFamily.pNext = nullptr;
    }
    vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, &queueFamilyCount, queueFamilies.data());
    std::cout << ANSI_BG_COLOR_CYAN << "3##############before loop" << ANSI_RESET << std::endl;

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        std::cout << "4##############before loop" << std::endl;
        if (indices.isComplete()) {
            break;
        }
        std::cout << "5##############before loop" << std::endl;
        i++;
    }

    return indices;
}

uint32_t scorePhysicalDevice(const VkPhysicalDevice& device) {
    VkPhysicalDeviceProperties2 deviceProperties{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, .pNext = nullptr};
    VkPhysicalDeviceFeatures2   deviceFeatures{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, .pNext = nullptr};
    vkGetPhysicalDeviceProperties2(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures2(device, &deviceFeatures);

    std::cout << "before find queueFamilies in score physical device" << std::endl;
    QueueFamilyIndices indices = findQueueFamilies(device);
    std::cout << "after find queueFamilies in score physical device" << std::endl;

    return indices.isComplete();
}

void pickPhysicalDevice(VkInstance instance, VkPhysicalDevice& physicalDevice) {
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
