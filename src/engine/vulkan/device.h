#pragma once
#include <vulkan/vulkan.h>

#include <vector>

void createLogicalDevice(VkDevice&                       device,
                         const VkPhysicalDevice          physicalDevice,
                         const std::vector<const char*>& validationLayers,
                         const std::vector<const char*>& deviceExtensions,
                         VkQueue&                        graphicsQueue,
                         VkQueue&                        presentQueue,
                         const VkSurfaceKHR              surface);
