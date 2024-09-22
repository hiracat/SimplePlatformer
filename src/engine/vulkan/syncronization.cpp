#include <vulkan/vulkan.h>

#include <cstddef>
#include <cstdint>
#include <vulkan/vulkan_core.h>

#include "../../utils/debugprint.h"
#include "../engine.h"
#include "syncronization.h"

void createSyncObjects(SyncResources& resources, const VkDevice& device, const uint32_t maxFramesInFlight) {
    resources.imageAvailable.resize(maxFramesInFlight);
    resources.renderFinished.resize(maxFramesInFlight);
    resources.inFlight.resize(maxFramesInFlight);
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < maxFramesInFlight; i++) {

        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &resources.imageAvailable[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &resources.renderFinished[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &resources.inFlight[i]) != VK_SUCCESS) {
            debugerror("failed to create sync objects");
        }
    }
}
void cleanupSyncObjects(SyncResources& resources, const VkDevice device, const uint32_t maxFramesInFlight) {

    for (size_t i = 0; i < maxFramesInFlight; i++) {
        vkDestroySemaphore(device, resources.imageAvailable[i], nullptr);
        vkDestroySemaphore(device, resources.renderFinished[i], nullptr);
        vkDestroyFence(device, resources.inFlight[i], nullptr);
    }
}
