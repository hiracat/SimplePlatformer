#pragma once

#include "../window.h"
#include <vector>
#include <vulkan/vulkan_core.h>

struct Swapchain {
    VkSwapchainKHR           swapchain;
    std::vector<VkImage>     images;
    VkFormat                 format;
    VkExtent2D               extent;
    std::vector<VkImageView> imageViews;
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentModes;
};
void createFramebuffers(std::vector<VkFramebuffer>&     swapchainFrameBuffers,
                        const std::vector<VkImageView>& swapchainImageViews,
                        const VkRenderPass              renderPass,
                        const VkExtent2D                swapchainExtent,
                        const VkDevice                  device);

void createSwapChain(const VkPhysicalDevice physicalDevice,
                     const VkSurfaceKHR     surface,
                     const Window           window,
                     Swapchain&             swapchain,
                     const VkDevice         device);
void cleanupSwapChain(Swapchain& swapchain, std::vector<VkFramebuffer>& swapchainFramebuffers, const VkDevice device);
void recreateSwapChain(const VkPhysicalDevice      physicalDevice,
                       const VkSurfaceKHR          surface,
                       const Window&               window,
                       Swapchain&                  swapchain,
                       const VkDevice              device,
                       const VkRenderPass          renderpass,
                       std::vector<VkFramebuffer>& swapchainFrameBuffers);
