#include <cstring>
#include <iostream>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vk_enum_string_helper.h>

#include "../engine/vulkan/physicaldevice.h"
#include "../engine/window.h"
#include "../game/main.h"
#include "../utils/debugprint.h"
#include "vulkan/commandobjects.h"
#include "vulkan/device.h"
#include "vulkan/graphicspipeline.h"
#include "vulkan/instance.h"
#include "vulkan/renderpass.h"
#include "vulkan/syncronization.h"
#include "vulkan/validationlayers.h"

#ifndef NDEBUG
#define ENABLE_VALIDATION_LAYERS
#endif

void initEngine(AppData& appdata) {
    debugnote("ig we're at it again yay have fun");
    glfwInit();

    initializeWindow(appdata.window.windowPointer, 800, 600, "window");
    createVkInstance(appdata.instance, appdata.validationLayers);
#ifdef ENABLE_VALIDATION_LAYERS
    setupDebugMessanger(appdata.instance, &appdata.debugMessenger);
#endif
    createSurface(appdata.instance, appdata.window.windowPointer, &appdata.window.surface);
    pickPhysicalDevice(appdata.instance, appdata.physicalDevice, appdata.window.surface, appdata.deviceExtensions);

    createLogicalDevice(appdata.device,
                        appdata.physicalDevice,
                        appdata.validationLayers,
                        appdata.deviceExtensions,
                        appdata.graphicsQueue,
                        appdata.presentQueue,
                        appdata.window.surface);

    createSwapChain(appdata.physicalDevice, appdata.window.surface, appdata.window, appdata.swapchain, appdata.device);
    createRenderPass(appdata.swapchain.format, appdata.renderPass, appdata.device);
    createGraphicsPipeline(
        appdata.device, appdata.swapchain, appdata.pipelineLayout, appdata.renderPass, appdata.graphicsPipeline);
    createFramebuffers(appdata.swapchainFramebuffers,
                       appdata.swapchain.imageViews,
                       appdata.renderPass,
                       appdata.swapchain.extent,
                       appdata.device);
    createCommandPool(appdata.physicalDevice, appdata.window.surface, appdata.commandPool, appdata.device);
    createCommandBuffer(appdata.device, appdata.commandPool, appdata.commandBuffer);
    createSyncObjects(appdata.syncObjects, appdata.device);

    debugnote("graphics queue: " << appdata.graphicsQueue);
    debugnote("present queue: " << appdata.presentQueue);
}
