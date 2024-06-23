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

    vkDeviceWaitIdle(data.vulkanObjects.device);

    cleanupSwapChain(data.renderingObjects.swapchain.swapchain,
                     data.resources.imageViews,
                     data.resources.swapchainFramebuffers,
                     data.vulkanObjects.device);
    vkDestroyBuffer(data.vulkanObjects.device, data.resources.vertexBuffer, nullptr);
    vkFreeMemory(data.vulkanObjects.device, data.resources.vertexBufferMemory, nullptr);

    vkDestroyPipeline(data.vulkanObjects.device, data.renderingObjects.graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(data.vulkanObjects.device, data.renderingObjects.pipelineLayout, nullptr);

    vkDestroyRenderPass(data.vulkanObjects.device, data.renderingObjects.renderPass, nullptr);

    cleanupSyncObjects(data.resources.syncObjects, data.vulkanObjects.device, data.resources.MAX_FRAMES_IN_FLIGHT);

    vkDestroyCommandPool(data.vulkanObjects.device, data.renderingObjects.commandPool, nullptr);

    vkDestroyDevice(data.vulkanObjects.device, nullptr);

#ifdef ENABLE_VALIDATION_LAYERS
    DestroyDebugUtilsMessengerEXT(data.vulkanObjects.instance, data.vulkanObjects.debugMessenger, nullptr);
#endif
    vkDestroySurfaceKHR(data.vulkanObjects.instance, data.window.surface, nullptr);
    vkDestroyInstance(data.vulkanObjects.instance, nullptr);

    glfwDestroyWindow(data.window.windowPointer);
    glfwTerminate();
}
