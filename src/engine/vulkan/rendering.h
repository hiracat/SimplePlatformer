#pragma once
#include "syncronization.h"
#include <vector>
#include <vulkan/vulkan_core.h>

void drawFrame(const VkDevice&                   device,
               const SyncObjects&                syncObjects,
               const VkSwapchainKHR&             swapchain,
               const VkCommandBuffer&            commandBuffer,
               const VkExtent2D&                 swapchainExtent,
               const VkRenderPass&               renderPass,
               const std::vector<VkFramebuffer>& swapchainFrameBuffers,
               const VkPipeline&                 graphicsPipeline,
               const VkQueue&                    graphicsQueue,
               const VkQueue&                    presentQueue);
