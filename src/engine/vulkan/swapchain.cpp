#include "swapchain.h"
#include "../../utils/debugprint.h"
#include "../window.h"
#include "physicaldevice.h"
#include <vector>
// #include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

void createImageViews(std::vector<VkImageView>&   imageViews,
                      const std::vector<VkImage>& images,
                      VkFormat                    swapchainFormat,
                      const VkDevice&             device) {
    imageViews.resize(images.size());
    for (size_t i = 0; i < images.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image                           = images[i];
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
        if (vkCreateImageView(device, &createInfo, nullptr, &imageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views");
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

void recreateSwapChain(const VkPhysicalDevice      physicalDevice,
                       const VkSurfaceKHR          surface,
                       const Window&               window,
                       Swapchain&                  swapchain,
                       const VkDevice              device,
                       const VkRenderPass          renderpass,
                       std::vector<VkFramebuffer>& swapchainFrameBuffers) {

    int width = 0, height = 0;
    glfwGetFramebufferSize(window.windowPointer, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window.windowPointer, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device);
    VkSwapchainKHR oldSwapchain = swapchain.swapchain;

    createSwapChain(physicalDevice, surface, window, swapchain, device, oldSwapchain);
    cleanupSwapChain(oldSwapchain, swapchain.imageViews, swapchainFrameBuffers, device);
    createImageViews(swapchain.imageViews, swapchain.images, swapchain.format, device);
    createFramebuffers(swapchainFrameBuffers, swapchain.imageViews, renderpass, swapchain.extent, device);
}

void createFramebuffers(std::vector<VkFramebuffer>&     swapchainFrameBuffers,
                        const std::vector<VkImageView>& swapchainImageViews,
                        const VkRenderPass              renderPass,
                        const VkExtent2D                swapchainExtent,
                        const VkDevice                  device) {

    swapchainFrameBuffers.resize(swapchainImageViews.size());

    for (size_t i = 0; i < swapchainImageViews.size(); i++) {
        VkImageView             attachments[] = {swapchainImageViews[i]}; // as an array because there can be multiple attachments
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass      = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments    = attachments;
        framebufferInfo.width           = swapchainExtent.width;
        framebufferInfo.height          = swapchainExtent.height;
        framebufferInfo.layers          = 1;
        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapchainFrameBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer");
        }
    }
}

void createSwapChain(const VkPhysicalDevice physicalDevice,
                     const VkSurfaceKHR     surface,
                     const Window           window,
                     Swapchain&             swapchain,
                     const VkDevice         device,
                     VkSwapchainKHR&        oldSwapChain) {

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR   presentMode   = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D         extent        = chooseSwapExtent(swapChainSupport.capabilities, window);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (imageCount > swapChainSupport.capabilities.maxImageCount && swapChainSupport.capabilities.maxImageCount != 0) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount    = imageCount;
    createInfo.imageFormat      = surfaceFormat.format;
    createInfo.imageColorSpace  = surfaceFormat.colorSpace;
    createInfo.imageExtent      = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices              = findQueueFamilies(physicalDevice, surface);
    uint32_t           queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.presentFamily.value() == indices.graphicsFamily.value()) {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        debugnote("graphics and presentation queus are the same");
    } else {
        createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices   = queueFamilyIndices;
        debugnote("graphics and presentation queus are different");
    }
    createInfo.preTransform   = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode    = presentMode;
    createInfo.clipped        = VK_TRUE;
    createInfo.oldSwapchain   = oldSwapChain;

    VkResult result = VK_SUCCESS;

    if ((result = vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain.swapchain)) != VK_SUCCESS) {
        // debugerror(string_VkResult(result));
        throw std::runtime_error("failed to create swapchain");
    }

    vkGetSwapchainImagesKHR(device, swapchain.swapchain, &imageCount, nullptr);
    swapchain.images.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapchain.swapchain, &imageCount, swapchain.images.data());

    swapchain.format = surfaceFormat.format;
    swapchain.extent = extent;
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
    debugnote("swapchain min images from in query swapchain support: " << details.capabilities.minImageCount);
    return details;
}
