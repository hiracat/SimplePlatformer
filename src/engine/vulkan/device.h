#pragma once
#include "../enginedata.h"

void createDevice(InstanceResources resources,
                  VkDevice& device, // has to be a reference otherwise it only changes the device pointer copy and not the real thing
                  const VkPhysicalDevice physicalDevice,
                  Queues&                queues, // same with this
                  const VkSurfaceKHR     surface,
                  QueueFamilyIndices&    indices); // and this
