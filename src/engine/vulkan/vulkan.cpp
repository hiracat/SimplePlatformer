#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vulkan/vulkan_core.h>
#include "../enginedata.h"
#include "commandobjects.h"
#include "device.h"
#include "graphicspipeline.h"
#include "instance.h"
#include "physicaldevice.h"
#include "renderpass.h"
#include "../vertex.h"
#include "buffers.h"
#include "../../compilesettings.h"
#include "validationlayers.h"

void initVulkan(EngineData& enginedata, const std::vector<Vertex>& renderData){

    createVkInstance(enginedata.instance, enginedata.validationLayers);
#ifdef ENABLE_VALIDATION_LAYERS
    setupDebugMessanger(enginedata.instance, &enginedata.debugMessenger);
#endif
    if (glfwCreateWindowSurface(enginedata.instance, enginedata.window.windowPointer, nullptr, &enginedata.window.surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface");
    }
    pickPhysicalDevice(enginedata.instance, enginedata.physicalDevice, enginedata.window.surface, enginedata.deviceExtensions);

    createLogicalDevice(enginedata.device,
                        enginedata.physicalDevice,
                        enginedata.validationLayers,
                        enginedata.deviceExtensions,
                        enginedata.graphicsQueue,
                        enginedata.presentQueue,
                        enginedata.window.surface);

    VkSwapchainKHR oldSwapChain = VK_NULL_HANDLE;
    createSwapChain(
        enginedata.physicalDevice, enginedata.window.surface, enginedata.window, enginedata.swapchain, enginedata.device, oldSwapChain);
    createRenderPass(enginedata.swapchain.format, enginedata.renderPass, enginedata.device);
    createGraphicsPipeline(
        enginedata.device, enginedata.swapchain, enginedata.pipelineLayout, enginedata.renderPass, enginedata.graphicsPipeline);
    createImageViews(enginedata.swapchain.imageViews, enginedata.swapchain.images, enginedata.swapchain.format, enginedata.device);
    createFramebuffers(enginedata.swapchainFramebuffers,
                       enginedata.swapchain.imageViews,
                       enginedata.renderPass,
                       enginedata.swapchain.extent,
                       enginedata.device);
    createCommandPool(enginedata.physicalDevice, enginedata.window.surface, enginedata.commandPool, enginedata.device);
    createVertexBuffer(renderData, enginedata.device, enginedata.physicalDevice, enginedata.vertexBuffer, enginedata.vertexBufferMemory);
    createCommandBuffers(enginedata.device, enginedata.commandPool, enginedata.commandBuffers, enginedata.MAX_FRAMES_IN_FLIGHT);
    createSyncObjects(enginedata.syncObjects, enginedata.device, enginedata.MAX_FRAMES_IN_FLIGHT);
}
