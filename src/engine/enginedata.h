#pragma once

#include <vulkan/vulkan.h>

#include <cstdint>
#include <vector>

#include "vertex.h"
#include "vulkan/swapchain.h"
#include "vulkan/syncronization.h"
#include "window.h"

struct VulkanObjects {
    VkInstance                     instance{};
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    VkDebugUtilsMessengerEXT       debugMessenger{};
    VkPhysicalDevice               physicalDevice   = VK_NULL_HANDLE;
    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    VkQueue                        graphicsQueue{};
    VkQueue                        presentQueue{};
    VkQueue                        transferQueue{};
    VkDevice                       device{};
};

struct RenderingObjects {
    Swapchain        swapchain{};
    VkSwapchainKHR   oldSwapChain{};
    VkRenderPass     renderPass{};
    VkPipeline       graphicsPipeline{};
    VkCommandPool    commandPool{};
    VkPipelineLayout pipelineLayout{};
};

struct Model {

    std::vector<Vertex> vertices = {{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};

    std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};
};

struct RenderingResources {
    std::vector<VkImage>         images;
    std::vector<VkImageView>     imageViews;
    std::vector<VkFramebuffer>   swapchainFramebuffers{};
    Model                        renderData;
    VkBuffer                     vertexBuffer{};
    VkDeviceMemory               vertexBufferMemory{};
    VkBuffer                     indexBuffer{};
    VkDeviceMemory               indexBufferMemory{};
    std::vector<VkCommandBuffer> commandBuffers{};
    SyncObjects                  syncObjects{};

    const int MAX_FRAMES_IN_FLIGHT = 2;
};

struct Data {

    VulkanObjects      vulkanObjects;
    RenderingObjects   renderingObjects;
    RenderingResources resources;

    Window   window{};
    uint32_t currentFrame       = 0;
    bool     framebufferResized = false;
};
