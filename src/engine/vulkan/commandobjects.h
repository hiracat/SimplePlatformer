#pragma once
#include <vulkan/vulkan_core.h>

void createCommandBuffer(const VkDevice& device, const VkCommandPool& commandPool, VkCommandBuffer& commandBuffer);
void createCommandPool(const VkPhysicalDevice& physicalDevice,
                       const VkSurfaceKHR&     surface,
                       VkCommandPool&          commandPool,
                       const VkDevice&         device);
void recordCommandBuffer(VkCommandBuffer      commandBuffer,
                         const VkExtent2D&    swapChainExtent,
                         const VkRenderPass&  renderPass,
                         const VkFramebuffer& swapchainFrameBuffer,
                         const VkPipeline&    graphicsPipeline);
