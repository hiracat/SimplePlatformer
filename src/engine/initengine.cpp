#include <cstring>
#include <iostream>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vk_enum_string_helper.h>

#include "../compilesettings.h"
#include "../engine/vulkan/physicaldevice.h"
#include "../engine/window.h"
#include "../utils/debugprint.h"
#include "enginedata.h"
#include "vulkan/commandobjects.h"
#include "vulkan/device.h"
#include "vulkan/graphicspipeline.h"
#include "vulkan/instance.h"
#include "vulkan/renderpass.h"
#include "vulkan/syncronization.h"
#include "vulkan/validationlayers.h"

void initEngine(EngineData& enginedata) {
    debugnote("ig we're at it again yay have fun");
    glfwInit();

    initializeWindow(enginedata.window.windowPointer, 800, 600, "window", &enginedata.framebufferResized);
    createVkInstance(enginedata.instance, enginedata.validationLayers);
#ifdef ENABLE_VALIDATION_LAYERS
    setupDebugMessanger(enginedata.instance, &enginedata.debugMessenger);
#endif
    createSurface(enginedata.instance, enginedata.window.windowPointer, &enginedata.window.surface);
    pickPhysicalDevice(enginedata.instance, enginedata.physicalDevice, enginedata.window.surface, enginedata.deviceExtensions);

    createLogicalDevice(enginedata.device,
                        enginedata.physicalDevice,
                        enginedata.validationLayers,
                        enginedata.deviceExtensions,
                        enginedata.graphicsQueue,
                        enginedata.presentQueue,
                        enginedata.window.surface);

    createSwapChain(enginedata.physicalDevice, enginedata.window.surface, enginedata.window, enginedata.swapchain, enginedata.device);
    createRenderPass(enginedata.swapchain.format, enginedata.renderPass, enginedata.device);
    createGraphicsPipeline(
        enginedata.device, enginedata.swapchain, enginedata.pipelineLayout, enginedata.renderPass, enginedata.graphicsPipeline);
    createFramebuffers(enginedata.swapchainFramebuffers,
                       enginedata.swapchain.imageViews,
                       enginedata.renderPass,
                       enginedata.swapchain.extent,
                       enginedata.device);
    createCommandPool(enginedata.physicalDevice, enginedata.window.surface, enginedata.commandPool, enginedata.device);
    createCommandBuffers(enginedata.device, enginedata.commandPool, enginedata.commandBuffers, enginedata.MAX_FRAMES_IN_FLIGHT);
    createSyncObjects(enginedata.syncObjects, enginedata.device, enginedata.MAX_FRAMES_IN_FLIGHT);

    debugnote("graphics queue: " << enginedata.graphicsQueue);
    debugnote("present queue: " << enginedata.presentQueue);
    debugnote("swapchain: " << enginedata.swapchain.swapchain);
}
