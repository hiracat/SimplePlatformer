#pragma once
#include "../enginedata.h"

void createDevice(VkDevice& device, // has to be a reference otherwise it only changes the device pointer copy and not the real thing
                  const VkPhysicalDevice physicalDevice,
                  InstanceResources      resources,
                  Queues&                 queues, // same with this
                  const VkSurfaceKHR     surface);
