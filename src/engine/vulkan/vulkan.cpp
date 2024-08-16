#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vulkan/vulkan_core.h>

#include "../../compilesettings.h"
#include "../enginedata.h"
#include "buffers.h"
#include "commandobjects.h"
#include "device.h"
#include "graphicspipeline.h"
#include "instance.h"
#include "physicaldevice.h"
#include "renderpass.h"
#include "swapchain.h"
#include "validationlayers.h"

void initVulkan(VulkanObjects& vulkanObjects, Window& window) {
    createVkInstance(vulkanObjects.instance, vulkanObjects.validationLayers);

#ifdef ENABLE_VALIDATION_LAYERS
    setupDebugMessanger(vulkanObjects.instance, &vulkanObjects.debugMessenger);
#endif
    if (glfwCreateWindowSurface(vulkanObjects.instance, window.windowPointer, nullptr, &window.surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface");
    }
    pickPhysicalDevice(vulkanObjects.instance, vulkanObjects.physicalDevice, window.surface, vulkanObjects.deviceExtensions);

    createLogicalDevice(vulkanObjects.device,
                        vulkanObjects.physicalDevice,
                        vulkanObjects.validationLayers,
                        vulkanObjects.deviceExtensions,
                        vulkanObjects.graphicsQueue,
                        vulkanObjects.presentQueue,
                        vulkanObjects.transferQueue,
                        window.surface);
}

void createRenderingObjects(RenderingObjects&      renderingObjects,
                            RenderingResources&    resources,
                            const VkDevice         device,
                            const VkPhysicalDevice physicalDevice,
                            const Window&          window,
                            const VkSurfaceKHR     surface,
                            const VkQueue          transferQueue) {

    createSwapChain(physicalDevice,
                    window.surface,
                    window,
                    renderingObjects.swapchain,
                    device,
                    renderingObjects.oldSwapChain,
                    resources.images,
                    resources.imageViews);
    createRenderPass(renderingObjects.swapchain.format, renderingObjects.renderPass, device);
    createGraphicsPipeline(device,
                           renderingObjects.swapchain,
                           renderingObjects.pipelineLayout,
                           renderingObjects.renderPass,
                           renderingObjects.graphicsPipeline);

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
    createCommandPool(physicalDevice, indices.graphicsFamily.value(), device, window.surface, renderingObjects.commandPool);
    if (indices.graphicsFamily.value() != indices.presentFamily.value()) {
        createCommandPool(physicalDevice, indices.presentFamily.value(), device, window.surface, renderingObjects.commandPool);
    }
    createImageViews(resources.imageViews, resources.images, renderingObjects.swapchain.format, device);
    createFramebuffers(
        resources.swapchainFramebuffers, resources.imageViews, renderingObjects.renderPass, renderingObjects.swapchain.extent, device);

    createIndexBuffer(resources.renderData.indices,
                      device,
                      physicalDevice,
                      resources.indexBuffer,
                      resources.indexBufferMemory,
                      surface,
                      renderingObjects.commandPool,
                      transferQueue);

    createVertexBuffer(resources.renderData.vertices,
                       device,
                       physicalDevice,
                       resources.vertexBuffer,
                       resources.vertexBufferMemory,
                       surface,
                       renderingObjects.commandPool,
                       transferQueue);
    createCommandBuffers(device, renderingObjects.commandPool, resources.commandBuffers, resources.MAX_FRAMES_IN_FLIGHT);
    createSyncObjects(resources.syncObjects, device, resources.MAX_FRAMES_IN_FLIGHT);
}
