#include "../compilesettings.h"
#include "../engine/vulkan/commandobjects.h"
#include "../engine/vulkan/device.h"
#include "../engine/vulkan/graphicspipeline.h"
#include "../engine/vulkan/instance.h"
#include "../engine/vulkan/physicaldevice.h"
#include "../engine/vulkan/renderpass.h"
#include "../engine/vulkan/validationlayers.h"
#include "../engine/window.h"
#include "../utils/debugprint.h"
#include "enginedata.h"
#include "vulkan/buffers.h"
#include "vulkan/swapchain.h"
#include "vulkan/syncronization.h"
#include "vulkan/vulkan.h"
#include "window.h"

void initEngine(Data& data) {
    createVkInstance(data.instance, data.instanceResources.validationLayers);
    initializeWindow(data.instance, data.windowResources, &data.framebufferResized);
    debugnote("created vk instance");
#ifdef ENABLE_VALIDATION_LAYERS
    setupDebugMessanger(data.instance, &data.instanceResources.debugMessenger);
    debugnote("setup debug messenger");
#endif
    pickPhysicalDevice(data.instance, data.physicalDevice, data.windowResources.surface, data.instanceResources.deviceExtensions);
    debugnote("picked physical device");
    createDevice(data.device, data.physicalDevice, data.instanceResources, data.queues, data.windowResources.surface);
    debugnote("created device");

    createSwapChain(data.physicalDevice, data.windowResources, data.swapchain, data.device, data.swapchainResources);

    createRenderPass(data.swapchain.format, data.pipelineResources.renderPass, data.device);
    createGraphicsPipeline(data.device, data.swapchain, data.pipelineResources);
    debugnote("created graphics pipeline");

    QueueFamilyIndices indices = findQueueFamilies(data.physicalDevice, data.windowResources.surface);
    createCommandPool(
        data.physicalDevice, indices.graphicsFamily.value(), data.device, data.windowResources.surface, data.commandResources.commandPool);
    if (indices.graphicsFamily.value() != indices.presentFamily.value()) {
        createCommandPool(data.physicalDevice,
                          indices.presentFamily.value(),
                          data.device,
                          data.windowResources.surface,
                          data.commandResources.commandPool);
    }
    createImageViews(data.swapchainResources, data.swapchain.format, data.device);
    createFramebuffers(data.swapchainResources, data.pipelineResources.renderPass, data.swapchain.extent, data.device);

    createCommandBuffers(data.device, data.commandResources, data.MAX_FRAMES_IN_FLIGHT);
    createSyncObjects(data.syncResources, data.device, data.MAX_FRAMES_IN_FLIGHT);
}
