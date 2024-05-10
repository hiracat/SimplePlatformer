#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

void createVkInstance(VkInstance& instance, const std::vector<const char*>& validationLayers);
