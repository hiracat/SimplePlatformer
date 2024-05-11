#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

struct SyncObjects {
    std::vector<VkSemaphore> imageAvailableSemaphores, renderFinishedSemaphores;
    std::vector<VkFence>     inFlightFences;
};

void createSyncObjects(SyncObjects& syncObjects, const VkDevice& device, const uint32_t maxFramesInFlight);
