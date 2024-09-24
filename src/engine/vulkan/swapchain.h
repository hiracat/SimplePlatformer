#pragma once
#include "../engine.h"
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

void createImageViews(const RendererData& renderData, SwapchainResources* resources);
void createFramebuffers(const RendererData& renderData, SwapchainResources* resources);

void createSwapChain(const RendererData& renderData, Swapchain* swapchain, SwapchainResources* swapchainResources);

void recreateSwapChain(const RendererData& renderData, Swapchain* swapchain, SwapchainResources* swapchainResources);

void cleanupSwapChain(VkSwapchainKHR&             swapchain,
                      std::vector<VkImageView>&   imageViews,
                      std::vector<VkFramebuffer>& swapchainFramebuffers,
                      const VkDevice              device);
