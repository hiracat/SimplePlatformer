
#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

#include "vulkan/swapchain.h"
#include "vulkan/syncronization.h"
#include "window.h"

struct EngineData {
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    VkDebugUtilsMessengerEXT       debugMessenger;
    Window                         window{};
    VkInstance                     instance{};
    VkDevice                       device;
    VkPhysicalDevice               physicalDevice = VK_NULL_HANDLE;
    VkQueue                        graphicsQueue;
    VkQueue                        presentQueue;
    Swapchain                      swapchain;
    VkPipelineLayout               pipelineLayout;
    VkRenderPass                   renderPass;
    VkPipeline                     graphicsPipeline;
    std::vector<VkFramebuffer>     swapchainFramebuffers;
    VkCommandPool                  commandPool;
    VkCommandBuffer                commandBuffer;
    SyncObjects                    syncObjects;
};
