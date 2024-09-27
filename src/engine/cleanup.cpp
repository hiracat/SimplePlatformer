#include <cstddef>
#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "../compilesettings.h"
#include "engine.h"
#include "vulkan/swapchain.h"
#include "vulkan/syncronization.h"

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT pMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        return func(instance, pMessenger, pAllocator);
    }
}

void cleanupEngine(EngineData* data, std::vector<Model>* models) {

    vkDeviceWaitIdle(data->vulkanData.device);

    cleanupSwapChain(data->renderData.swapchain.swapchain,
                     data->renderData.swapchainResources.imageViews,
                     data->renderData.swapchainResources.framebuffers,
                     data->vulkanData.device);

    for (size_t i = 0; i < data->MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(data->vulkanData.device, data->renderData.transformResources.uniformBuffers[i].buffer.buffer, nullptr);
        vkFreeMemory(data->vulkanData.device, data->renderData.transformResources.uniformBuffers[i].buffer.memory, nullptr);
    }
    vkDestroyDescriptorPool(data->vulkanData.device, data->renderData.descriptorResources.pool, nullptr);
    vkDestroyDescriptorSetLayout(data->vulkanData.device, data->renderData.transformResources.descriptorSetLayout, nullptr);

    for (size_t i = 0; i < models->size(); i++) {
        vkDestroyBuffer(data->vulkanData.device, (*models)[i].vertexBuffer.buffer, nullptr);
        vkFreeMemory(data->vulkanData.device, (*models)[i].vertexBuffer.memory, nullptr);

        vkDestroyBuffer(data->vulkanData.device, (*models)[i].indexBuffer.buffer, nullptr);
        vkFreeMemory(data->vulkanData.device, (*models)[i].indexBuffer.memory, nullptr);
    }

    vkDestroyPipeline(data->vulkanData.device, data->renderData.pipelineResources.graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(data->vulkanData.device, data->renderData.pipelineResources.pipelineLayout, nullptr);

    vkDestroyRenderPass(data->vulkanData.device, data->renderData.pipelineResources.renderPass, nullptr);

    cleanupSyncObjects(data->renderData.syncResources, data->vulkanData.device, data->MAX_FRAMES_IN_FLIGHT);

    vkDestroyCommandPool(data->vulkanData.device, data->renderData.commandResources.pool, nullptr);

    vkDestroyDevice(data->vulkanData.device, nullptr);

#ifdef ENABLE_VALIDATION_LAYERS
    DestroyDebugUtilsMessengerEXT(data->vulkanData.instance, data->vulkanData.debugMessenger, nullptr);
#endif
    vkDestroySurfaceKHR(data->vulkanData.instance, data->windowData.surface, nullptr);
    vkDestroyInstance(data->vulkanData.instance, nullptr);

    glfwDestroyWindow(data->windowData.window);
    glfwTerminate();
}
