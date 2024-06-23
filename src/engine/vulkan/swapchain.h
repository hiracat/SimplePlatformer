#pragma once
#include <vector>
#include <vulkan/vulkan.h>

#include "../window.h"

struct Swapchain {
    VkSwapchainKHR swapchain;
    VkFormat       format;
    VkExtent2D     extent;
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentModes;
};

void createImageViews(std::vector<VkImageView>&   imageViews,
                      const std::vector<VkImage>& images,
                      VkFormat                    swapchainFormat,
                      const VkDevice&             device);
void createFramebuffers(std::vector<VkFramebuffer>&     swapchainFrameBuffers,
                        const std::vector<VkImageView>& swapchainImageViews,
                        const VkRenderPass              renderPass,
                        const VkExtent2D                swapchainExtent,
                        const VkDevice                  device);

void createSwapChain(const VkPhysicalDevice    physicalDevice,
                     const VkSurfaceKHR        surface,
                     const Window&             window,
                     Swapchain&                swapchain,
                     const VkDevice            device,
                     VkSwapchainKHR&           oldSwapChain,
                     std::vector<VkImage>&     images,
                     std::vector<VkImageView>& imageViews);

void recreateSwapChain(const VkPhysicalDevice      physicalDevice,
                       const Window&               window,
                       Swapchain&                  swapchain,
                       const VkDevice              device,
                       const VkRenderPass          renderpass,
                       std::vector<VkFramebuffer>& swapchainFrameBuffers,
                       std::vector<VkImage>&       images,
                       std::vector<VkImageView>&   imageViews);
void cleanupSwapChain(VkSwapchainKHR&             swapchain,
                      std::vector<VkImageView>&   imageViews,
                      std::vector<VkFramebuffer>& swapchainFramebuffers,
                      const VkDevice              device);
