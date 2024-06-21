#pragma once
#include <vulkan/vulkan.h>

void createRenderPass(const VkFormat& swapchainFormat, VkRenderPass& renderPass, const VkDevice& device);
