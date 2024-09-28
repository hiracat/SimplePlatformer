#include "engine.h"
#include "../compilesettings.h"
#include "../engine/vulkan/commandobjects.h"
#include "../engine/vulkan/device.h"
#include "../engine/vulkan/graphicspipeline.h"
#include "../engine/vulkan/instance.h"
#include "../engine/vulkan/physicaldevice.h"
#include "../engine/vulkan/renderpass.h"
#include "../engine/vulkan/ubo.h"
#include "../engine/window.h"
#include "../utils/debugprint.h"
#include "engine.h"
#include "vulkan/buffers.h"
#include "vulkan/swapchain.h"
#include "vulkan/syncronization.h"
#include "window.h"
#include <GLFW/glfw3.h>
#include <array>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

VkVertexInputBindingDescription Vertex::getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};

    bindingDescription.binding   = 0;
    bindingDescription.stride    = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}
std::array<VkVertexInputAttributeDescription, 2> Vertex::getAttributeDescriptions() {

    // two attribe discriptiors for each of the data types stored in the vertex
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

    attributeDescriptions[0].binding  = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format   = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset   = offsetof(Vertex, pos);

    attributeDescriptions[1].binding  = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset   = offsetof(Vertex, color);

    return attributeDescriptions;
}
void initEngine(EngineData* engine) {
    // needs to be called here because we use a function in vkcreateinstance to get the extensions needed for glfw to work
    int reply = glfwInit();
    debugnote("reply: " << reply);

    createInstance(engine->vulkanData, &engine->vulkanData.instance);
    initWindow(engine->vulkanData, &engine->windowData);
    debugnote("created vk instance");
#ifdef ENABLE_VALIDATION_LAYERS
    setupDebugMessanger(engine->vulkanData, &engine->vulkanData.debugMessenger);
    debugnote("setup debug messenger");
#endif
    pickPhysicalDevice(engine->vulkanData, &engine->vulkanData.physicalDevice);
    // cache queue families so we dont have to find them again for every use
    findQueueFamilies(engine->vulkanData, VK_NULL_HANDLE, &engine->vulkanData.queueFamilyIndices);
    debugnote("picked physical device");

    createDevice(engine->vulkanData, &engine->vulkanData.device, &engine->vulkanData.queues);
    debugnote("created device");

    createSwapChain(engine->renderData, &engine->renderData.swapchain, &engine->renderData.swapchainResources);

    createRenderPass(engine->renderData, &engine->renderData.pipelineResources.renderPass);

    createDescriptorSetLayout(engine->vulkanData.device, &engine->renderData.transformResources.descriptorSetLayout);

    createGraphicsPipeline(
        engine->renderData, &engine->renderData.pipelineResources, &engine->renderData.transformResources.descriptorSetLayout);
    debugnote("created graphics pipeline");

    createCommandPool(
        engine->vulkanData, engine->vulkanData.queueFamilyIndices.graphics.value(), &engine->renderData.commandResources.pool);
    if (engine->vulkanData.queueFamilyIndices.graphics.value() != engine->vulkanData.queueFamilyIndices.present.value()) {
        createCommandPool(
            engine->vulkanData, engine->vulkanData.queueFamilyIndices.graphics.value(), &engine->renderData.commandResources.pool);
    }
    createImageViews(engine->renderData, &engine->renderData.swapchainResources);
    createFramebuffers(engine->renderData, &engine->renderData.swapchainResources);

    createCommandBuffers(engine->vulkanData.device, &engine->renderData.commandResources, engine->MAX_FRAMES_IN_FLIGHT);
    createSyncObjects(engine->vulkanData.device, &engine->renderData.syncResources, engine->MAX_FRAMES_IN_FLIGHT);
}
