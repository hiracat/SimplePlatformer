#include <vulkan/vulkan.h>

#include <cstddef>
#include <cstdint>
#include <stdexcept>

#include "syncronization.h"

void createSyncObjects(SyncObjects& syncObjects, const VkDevice& device, const uint32_t maxFramesInFlight) {
    syncObjects.imageAvailableSemaphores.resize(maxFramesInFlight);
    syncObjects.renderFinishedSemaphores.resize(maxFramesInFlight);
    syncObjects.inFlightFences.resize(maxFramesInFlight);
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < maxFramesInFlight; i++) {

        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &syncObjects.imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &syncObjects.renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &syncObjects.inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create sync objects");
        }
    }
}
