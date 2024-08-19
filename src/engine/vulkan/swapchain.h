#pragma once
#include "../enginedata.h"
#include <vector>
#include <vulkan/vulkan.h>

void createImageViews(SwapchainResources& resources, VkFormat swapchainFormat, const VkDevice& device);
void createFramebuffers(SwapchainResources& resources,
                        const VkRenderPass  renderPass,
                        const VkExtent2D    swapchainExtent,
                        const VkDevice      device);

void createSwapChain(const VkPhysicalDevice physicalDevice,
                     const WindowResources& window,
                     Swapchain&             swapchain,
                     const VkDevice         device,
                     SwapchainResources&    swapchainResources);

void recreateSwapChain(const VkPhysicalDevice physicalDevice,
                       const WindowResources& window,
                       Swapchain&             swapchain,
                       const VkDevice         device,
                       const VkRenderPass     renderpass,
                       SwapchainResources&    resources);

void cleanupSwapChain(VkSwapchainKHR&             swapchain,
                      std::vector<VkImageView>&   imageViews,
                      std::vector<VkFramebuffer>& swapchainFramebuffers,
                      const VkDevice              device);
