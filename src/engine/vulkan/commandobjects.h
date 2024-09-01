#pragma once
#include "../enginedata.h"
#include <cstdint>
#include <vulkan/vulkan.h>

void createCommandBuffers(const VkDevice& device, CommandResources& resources, const uint32_t maxFramesInFlight);

void createCommandPool(const VkPhysicalDevice& physicalDevice,
                       const uint32_t          queueFamilyIndex,
                       const VkDevice&         device,
                       const VkSurfaceKHR&     surface,
                       VkCommandPool&          commandPool);

void recordCommandBuffer(VkCommandBuffer&    commandBuffer,
                         const VkExtent2D&   swapChainExtent,
                         const VkRenderPass& renderPass,
                         VkFramebuffer&      frameBuffer,
                         const VkPipeline&   graphicsPipeline,
                         const VkBuffer&     vertexBuffer,
                         const VkBuffer&     indexBuffer,

                         const VkBuffer& vertexBufferf,
                         const VkBuffer& indexBufferf,

                         VkDescriptorSet&        descriptorSet,
                         const VkPipelineLayout& pipelineLayout,
                         const uint32_t&         indicesCount,
                         const uint32_t&         indicesCountf);
