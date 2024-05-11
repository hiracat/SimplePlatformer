#pragma once
#include "swapchain.h"
#include "syncronization.h"
#include <vector>
#include <vulkan/vulkan_core.h>

void drawFrame(const VkDevice                      device,
               const SyncObjects&                  syncObjects,
               Swapchain&                          swapchain,
               const std::vector<VkCommandBuffer>& commandBuffers,
               const VkExtent2D&                   swapchainExtent,
               const VkRenderPass                  renderPass,
               std::vector<VkFramebuffer>&         swapchainFrameBuffers,
               const VkPipeline                    graphicsPipeline,
               const VkQueue                       graphicsQueue,
               const VkQueue                       presentQueue,
               uint32_t&                           currentFrame,
               const uint32_t                      maxFramesInFlight,
               const Window&                       window,
               const VkPhysicalDevice              physicalDevice,
               const VkSurfaceKHR                  surface,
               bool&                               frameBufferResized);
