#include "engine.h"
#include "../compilesettings.h"
#include "../engine/vulkan/commandobjects.h"
#include "../engine/vulkan/descriptorsets.h"
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

void initEngine(EngineData& engine) {
    glfwInit(); // needs to be called here because we use a function in vkcreateinstance to get the extensions needed for glfw to work

    createInstance(engine.vulkanData, &engine.vulkanData.instance);
    initWindow(engine.vulkanData, &engine.windowData);
    debugnote("created vk instance");
#ifdef ENABLE_VALIDATION_LAYERS
    setupDebugMessanger(engine.vulkanData, &engine.vulkanData.debugMessenger);
    debugnote("setup debug messenger");
#endif
    pickPhysicalDevice(engine.vulkanData, &engine.vulkanData.physicalDevice);
    findQueueFamilies(engine.vulkanData, &engine.vulkanData.queueFamilyIndices);
    debugnote("picked physical device");

    createDevice(engine.vulkanData, &engine.vulkanData.device, &engine.vulkanData.queues);
    debugnote("created device");

    createSwapChain(engine.device,
                    engine.physicalDevice,
                    engine.windowResources,
                    engine.swapchain,
                    engine.swapchainResources,
                    engine.queueFamilyIndices);

    createRenderPass(engine.swapchain.format, engine.pipelineResources.renderPass, engine.device);
    CreateDescriptorSetLayout(engine.device, engine.transformResources.descriptorSetLayout);
    createGraphicsPipeline(engine.device, engine.swapchain, engine.pipelineResources, engine.transformResources.descriptorSetLayout);
    debugnote("created graphics pipeline");

    createCommandPool(engine.physicalDevice,
                      engine.queueFamilyIndices.graphics.value(),
                      engine.device,
                      engine.windowResources.surface,
                      engine.commandResources.pool);
    if (engine.queueFamilyIndices.graphics.value() != engine.queueFamilyIndices.present.value()) {
        createCommandPool(engine.physicalDevice,
                          engine.queueFamilyIndices.present.value(),
                          engine.device,
                          engine.windowResources.surface,
                          engine.commandResources.pool);
    }
    createImageViews(engine.swapchainResources, engine.swapchain.format, engine.device);
    createFramebuffers(engine.swapchainResources, engine.pipelineResources.renderPass, engine.swapchain.extent, engine.device);

    createCommandBuffers(engine.device, engine.commandResources, engine.MAX_FRAMES_IN_FLIGHT);
    createSyncObjects(engine.syncResources, engine.device, engine.MAX_FRAMES_IN_FLIGHT);
}
