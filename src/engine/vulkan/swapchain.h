#pragma once
#include "../engine.h"
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

void createImageViews(SwapchainResources& resources, VkFormat swapchainFormat, const VkDevice& device);
void createFramebuffers(SwapchainResources& resources,
                        const VkRenderPass  renderPass,
                        const VkExtent2D    swapchainExtent,
                        const VkDevice      device);

void createSwapChain(const VulkanData& vulkanData, Swapchain* swapchain, SwapchainResources* swapchainResources);

void recreateSwapChain(const VulkanData&   vulkanData,
                       const VkRenderPass  renderPass,
                       Swapchain*          swapchain,
                       SwapchainResources* swapchainResources);

void cleanupSwapChain(VkSwapchainKHR&             swapchain,
                      std::vector<VkImageView>&   imageViews,
                      std::vector<VkFramebuffer>& swapchainFramebuffers,
                      const VkDevice              device);
