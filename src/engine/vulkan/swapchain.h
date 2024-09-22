#pragma once
#include "../engine.h"
#include <vector>
#include <vulkan/vulkan.h>

void createImageViews(SwapchainResources& resources, VkFormat swapchainFormat, const VkDevice& device);
void createFramebuffers(SwapchainResources& resources,
                        const VkRenderPass  renderPass,
                        const VkExtent2D    swapchainExtent,
                        const VkDevice      device);

void createSwapChain(const VkDevice            device,
                     const VkPhysicalDevice    physicalDevice,
                     const WindowData&         window,
                     Swapchain&                swapchain,
                     SwapchainResources&       swapchainResources,
                     const QueueFamilyIndices& indices);

void recreateSwapChain(const VkPhysicalDevice    physicalDevice,
                       const WindowData&         window,
                       Swapchain&                swapchain,
                       const VkDevice            device,
                       const VkRenderPass        renderpass,
                       SwapchainResources&       resources,
                       const QueueFamilyIndices& indices);

void cleanupSwapChain(VkSwapchainKHR&             swapchain,
                      std::vector<VkImageView>&   imageViews,
                      std::vector<VkFramebuffer>& swapchainFramebuffers,
                      const VkDevice              device);
