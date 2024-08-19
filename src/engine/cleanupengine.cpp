#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include "../compilesettings.h"
#include "enginedata.h"
#include "vulkan/swapchain.h"
#include "vulkan/syncronization.h"

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT pMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        return func(instance, pMessenger, pAllocator);
    }
}

void cleanup(Data& data) {

    vkDeviceWaitIdle(data.device);

    cleanupSwapChain(
        data.swapchain.swapchain, data.swapchainResources.imageViews, data.swapchainResources.swapchainFramebuffers, data.device);

    vkDestroyBuffer(data.device, data.indexBuffer.buffer, nullptr);
    vkFreeMemory(data.device, data.indexBuffer.memory, nullptr);

    vkDestroyBuffer(data.device, data.vertexBuffer.buffer, nullptr);
    vkFreeMemory(data.device, data.vertexBuffer.memory, nullptr);

    vkDestroyPipeline(data.device, data.pipelineResources.graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(data.device, data.pipelineResources.pipelineLayout, nullptr);

    vkDestroyRenderPass(data.device, data.pipelineResources.renderPass, nullptr);

    cleanupSyncObjects(data.syncResources, data.device, data.MAX_FRAMES_IN_FLIGHT);

    vkDestroyCommandPool(data.device, data.commandResources.commandPool, nullptr);

    vkDestroyDevice(data.device, nullptr);

#ifdef ENABLE_VALIDATION_LAYERS
    DestroyDebugUtilsMessengerEXT(data.instance, data.instanceResources.debugMessenger, nullptr);
#endif
    vkDestroySurfaceKHR(data.instance, data.windowResources.surface, nullptr);
    vkDestroyInstance(data.instance, nullptr);

    glfwDestroyWindow(data.windowResources.windowPointer);
    glfwTerminate();
}
