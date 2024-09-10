#include <cstdint>
#include <vulkan/vulkan.h>

#include <set>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "../../compilesettings.h"
#include "../../utils/debugprint.h"
#include "../engine.h"

void createDevice(const VulkanData& vulkanData, VkDevice* device, Queues* queues) {

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t>                   uniqueQueueFamilies = {vulkanData.queueFamilyIndices.graphics.value(),
                                                                vulkanData.queueFamilyIndices.present.value(),
                                                                vulkanData.queueFamilyIndices.transfer.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount       = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures2 deviceFeatures{};

    VkDeviceCreateInfo createInfo{
        .sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
        .pQueueCreateInfos    = queueCreateInfos.data(),

        .enabledExtensionCount   = static_cast<uint32_t>(vulkanData.deviceExtensions.size()),
        .ppEnabledExtensionNames = vulkanData.deviceExtensions.data(),
        .pEnabledFeatures        = &deviceFeatures.features,
    };

#ifdef ENABLE_VALIDATION_LAYERS
    createInfo.enabledLayerCount   = static_cast<uint32_t>(vulkanData.validationLayers.size());
    createInfo.ppEnabledLayerNames = vulkanData.validationLayers.data();
#else
    createInfo.enabledLayerCount = 0;
#endif

    VkResult result = vkCreateDevice(vulkanData.physicalDevice, &createInfo, nullptr, device);
    if (result != VK_SUCCESS) {
        debugerror("failed to creat device");
    }
    vkGetDeviceQueue(*device, vulkanData.queueFamilyIndices.graphics.value(), 0, &queues->graphics);
    vkGetDeviceQueue(*device, vulkanData.queueFamilyIndices.present.value(), 0, &queues->present);
    vkGetDeviceQueue(*device, vulkanData.queueFamilyIndices.transfer.value(), 0, &queues->transfer);
}
