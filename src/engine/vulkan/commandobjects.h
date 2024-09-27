#pragma once
#include "../engine.h"
#include <cstdint>
#include <vulkan/vulkan.h>

void createCommandBuffers(const VkDevice& device, CommandResources* resources, const uint32_t maxFramesInFlight);

void createCommandPool(const VulkanData& vulkanData, const uint32_t queueFamilyIndex, VkCommandPool* commandPool);

void beginDrawing(VkCommandBuffer&    commandBuffer,
                  const VkExtent2D&   swapChainExtent,
                  const VkRenderPass& renderPass,
                  VkFramebuffer&      frameBuffer,
                  const VkPipeline&   graphicsPipeline);

void endDrawing(VkCommandBuffer commandBuffer);
