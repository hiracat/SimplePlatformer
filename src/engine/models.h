#pragma once
#include "../game/gamedata.h"
#include "enginedata.h"
#include "vulkan/buffers.h"
#include <vulkan/vulkan_core.h>

void createModel(VkDevice                  device,
                 VkPhysicalDevice          physicalDevice,
                 VkQueue                   transferQueue,
                 VkCommandPool             commandPool,
                 const QueueFamilyIndices& queueFamilyIndices,
                 Model&                    model);
