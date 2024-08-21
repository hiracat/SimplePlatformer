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
#include <GLFW/glfw3.h>

void initEngine(Data& data) {
    glfwInit(); // needs to be called here because we use a function in vkcreateinstance to get the extensions needed for glfw to work

    createVkInstance(data.instance, data.instanceResources.validationLayers);
    initializeWindow(data.instance, data.windowResources, &data.framebufferResized);
    debugnote("created vk instance");
#ifdef ENABLE_VALIDATION_LAYERS
    setupDebugMessanger(data.instance, &data.instanceResources.debugMessenger);
    debugnote("setup debug messenger");
#endif
    pickPhysicalDevice(data.instance, data.instanceResources.deviceExtensions, data.physicalDevice, data.windowResources.surface);
    data.queueFamilyIndices = findQueueFamilies(data.physicalDevice, data.windowResources.surface);
    debugnote("picked physical device");
    createDevice(
        data.instanceResources, data.device, data.physicalDevice, data.queues, data.windowResources.surface, data.queueFamilyIndices);
    debugnote("created device");

    createSwapChain(
        data.device, data.physicalDevice, data.windowResources, data.swapchain, data.swapchainResources, data.queueFamilyIndices);

    createRenderPass(data.swapchain.format, data.pipelineResources.renderPass, data.device);
    createGraphicsPipeline(data.device, data.swapchain, data.pipelineResources);
    debugnote("created graphics pipeline");

    createCommandPool(data.physicalDevice,
                      data.queueFamilyIndices.graphicsFamily.value(),
                      data.device,
                      data.windowResources.surface,
                      data.commandResources.commandPool);
    if (data.queueFamilyIndices.graphicsFamily.value() != data.queueFamilyIndices.presentFamily.value()) {
        createCommandPool(data.physicalDevice,
                          data.queueFamilyIndices.presentFamily.value(),
                          data.device,
                          data.windowResources.surface,
                          data.commandResources.commandPool);
    }
    createImageViews(data.swapchainResources, data.swapchain.format, data.device);
    createFramebuffers(data.swapchainResources, data.pipelineResources.renderPass, data.swapchain.extent, data.device);

    createCommandBuffers(data.device, data.commandResources, data.MAX_FRAMES_IN_FLIGHT);
    createSyncObjects(data.syncResources, data.device, data.MAX_FRAMES_IN_FLIGHT);
}
