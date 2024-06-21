#pragma once
#include <vulkan/vulkan.h>

#include <vector>

VkShaderModule createShaderModule(const VkDevice device, const std::vector<char>& code);
