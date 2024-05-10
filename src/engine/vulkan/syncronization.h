#pragma once
#include <vulkan/vulkan_core.h>

struct SyncObjects {
    VkSemaphore imageAvailableSemaphore, renderFinishedSemaphore;
    VkFence     inFlightFence;
};

void createSyncObjects(SyncObjects& syncObjects, const VkDevice& device);
