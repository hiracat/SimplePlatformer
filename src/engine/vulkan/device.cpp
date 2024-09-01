#include <vulkan/vulkan.h>

#include <set>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "../../compilesettings.h"
#include "../../utils/debugprint.h"
#include "../enginedata.h"

void createDevice(InstanceResources resources,
                  VkDevice& device, // has to be a reference otherwise it only changes the device pointer copy and not the real thing
                  const VkPhysicalDevice physicalDevice,
                  Queues&                queues, // same with this
                  const VkSurfaceKHR     surface,
                  QueueFamilyIndices&    indices) { // and this

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphics.value(), indices.present.value(), indices.transfer.value()};

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

        .enabledExtensionCount   = static_cast<uint32_t>(resources.deviceExtensions.size()),
        .ppEnabledExtensionNames = resources.deviceExtensions.data(),
        .pEnabledFeatures        = &deviceFeatures.features,
    };

#ifdef ENABLE_VALIDATION_LAYERS
    createInfo.enabledLayerCount   = static_cast<uint32_t>(resources.validationLayers.size());
    createInfo.ppEnabledLayerNames = resources.validationLayers.data();
#else
    createInfo.enabledLayerCount = 0;
#endif

    VkResult result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
    if (result != VK_SUCCESS) {
        debugerror("failed to creat device");
    }

    vkGetDeviceQueue(device, indices.graphics.value(), 0, &queues.graphics);
    vkGetDeviceQueue(device, indices.present.value(), 0, &queues.present);
    vkGetDeviceQueue(device, indices.transfer.value(), 0, &queues.transfer);
}
