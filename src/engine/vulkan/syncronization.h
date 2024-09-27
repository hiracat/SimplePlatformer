#pragma once
#include "../engine.h"
#include <vulkan/vulkan.h>

void createSyncObjects(const VkDevice& device, SyncResources* resources, const uint32_t maxFramesInFlight);
void cleanupSyncObjects(SyncResources& objects, const VkDevice device, const uint32_t maxFramesInFlight);
