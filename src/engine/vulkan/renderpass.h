#pragma once
#include <vulkan/vulkan_core.h>

void createRenderPass(const VkFormat& swapchainFormat, VkRenderPass& renderPass, const VkDevice& device);
