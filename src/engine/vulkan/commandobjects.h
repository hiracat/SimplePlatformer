#pragma once
#include <vulkan/vulkan.h>

#include <vector>

void createCommandBuffers(const VkDevice&               device,
                          const VkCommandPool&          commandPool,
                          std::vector<VkCommandBuffer>& commandBuffers,
                          const uint32_t                maxFramesInFlight);

void createCommandPool(const VkPhysicalDevice& physicalDevice,
                       const VkSurfaceKHR&     surface,
                       VkCommandPool&          commandPool,
                       const VkDevice&         device);

void recordCommandBuffer(VkCommandBuffer     commandBuffer,
                         const VkExtent2D&   swapChainExtent,
                         const VkRenderPass& renderPass,
                         VkFramebuffer&      swapchainFrameBuffer,
                         const VkPipeline&   graphicsPipeline,
                         const VkBuffer      vertexBuffer,
                         const uint32_t      verticesCount);
