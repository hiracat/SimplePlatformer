#pragma once
#include <vulkan/vulkan.h>

#include <vector>

void createVkInstance(VkInstance& instance, const std::vector<const char*>& validationLayers);
