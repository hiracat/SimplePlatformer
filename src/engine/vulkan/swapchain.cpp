#include <cstdint>
#include <vulkan/vulkan.h>

#include <vector>
#include <vulkan/vulkan_core.h>

#include "../../utils/debugprint.h"
#include "../engine.h"
#include "physicaldevice.h"
#include "swapchain.h"

void createImageViews(SwapchainResources& resources, VkFormat swapchainFormat, const VkDevice& device) {
    resources.imageViews.resize(resources.images.size());
    for (size_t i = 0; i < resources.images.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image                           = resources.images[i];
        createInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format                          = swapchainFormat;
        createInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel   = 0;
        createInfo.subresourceRange.levelCount     = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount     = 1;
        if (vkCreateImageView(device, &createInfo, nullptr, &resources.imageViews[i]) != VK_SUCCESS) {
            debugerror("failed to create image views");
        }
    }
}

void cleanupSwapChain(VkSwapchainKHR&             swapchain,
                      std::vector<VkImageView>&   imageViews,
                      std::vector<VkFramebuffer>& swapchainFramebuffers,
                      const VkDevice              device) {
    for (auto framebuffer : swapchainFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
    for (auto& imageView : imageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(device, swapchain, nullptr);
}

void recreateSwapChain(const VulkanData&   vulkanData,
                       const VkRenderPass  renderPass,
                       Swapchain*          swapchain,
                       SwapchainResources* swapchainResources) {

    int width = 0, height = 0;
    glfwGetFramebufferSize(vulkanData.windowData.window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(vulkanData.windowData.window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(vulkanData.device);
    swapchain->oldSwapChain = swapchain->swapchain;

    createSwapChain(vulkanData, swapchain, swapchainResources);

    cleanupSwapChain(swapchain->oldSwapChain, swapchainResources->imageViews, swapchainResources->framebuffers, vulkanData.device);
    createImageViews(*swapchainResources, swapchain->format, vulkanData.device);
    createFramebuffers(*swapchainResources, renderPass, swapchain->extent, vulkanData.device);
}

void createFramebuffers(SwapchainResources& resources,
                        const VkRenderPass  renderPass,
                        const VkExtent2D    swapchainExtent,
                        const VkDevice      device) {

    resources.framebuffers.resize(resources.imageViews.size());

    for (size_t i = 0; i < resources.imageViews.size(); i++) {
        VkImageView             attachments[] = {resources.imageViews[i]}; // as an array because there can be multiple attachments
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass      = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments    = attachments;
        framebufferInfo.width           = swapchainExtent.width;
        framebufferInfo.height          = swapchainExtent.height;
        framebufferInfo.layers          = 1;
        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &resources.framebuffers[i]) != VK_SUCCESS) {
            debugerror("failed to create framebuffer");
        }
    }
}

void createSwapChain(const VulkanData& vulkanData, Swapchain* swapchain, SwapchainResources* swapchainResources) {

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(vulkanData.physicalDevice, vulkanData.windowData.surface);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR   presentMode   = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D         extent        = chooseSwapExtent(swapChainSupport.capabilities, vulkanData.windowData);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (imageCount > swapChainSupport.capabilities.maxImageCount && swapChainSupport.capabilities.maxImageCount != 0) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }
    debugdata("swapchainimagecout = " << imageCount);

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = vulkanData.windowData.surface;

    createInfo.minImageCount    = imageCount;
    createInfo.imageFormat      = surfaceFormat.format;
    createInfo.imageColorSpace  = surfaceFormat.colorSpace;
    createInfo.imageExtent      = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t rawIndices[] = {vulkanData.queueFamilyIndices.graphics.value(), vulkanData.queueFamilyIndices.present.value()};

    if ((vulkanData.queueFamilyIndices.transfer.value() == vulkanData.queueFamilyIndices.graphics.value()) &&
        (vulkanData.queueFamilyIndices.graphics.value() == vulkanData.queueFamilyIndices.present.value())) {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    } else {
        createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 3;
        createInfo.pQueueFamilyIndices   = rawIndices;
    }
    createInfo.preTransform   = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode    = presentMode;
    createInfo.clipped        = VK_TRUE;
    createInfo.oldSwapchain   = swapchain->oldSwapChain;

    VkResult result = VK_SUCCESS;

    if ((result = vkCreateSwapchainKHR(vulkanData.device, &createInfo, nullptr, &swapchain->swapchain)) != VK_SUCCESS) {
        debugerror("failed to create swapchain: " << result);
    }

    vkGetSwapchainImagesKHR(vulkanData.device, swapchain->swapchain, &imageCount, nullptr);
    swapchainResources->images.resize(imageCount);
    vkGetSwapchainImagesKHR(vulkanData.device, swapchain->swapchain, &imageCount, swapchainResources->images.data());

    swapchain->format = surfaceFormat.format;
    swapchain->extent = extent;
}

SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface) {
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

    uint32_t formatCount{};
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    if (formatCount) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModesCount{};
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, nullptr);
    if (presentModesCount) {
        details.presentModes.resize(presentModesCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModesCount, details.presentModes.data());
    }
    // debugnote("swapchain min images from in query swapchain support: " << details.capabilities.minImageCount);
    return details;
}
