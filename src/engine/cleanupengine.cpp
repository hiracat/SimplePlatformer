#include <GLFW/glfw3.h>
#include <cstddef>
#include <vulkan/vulkan_core.h>

#include "../compilesettings.h"
#include "enginedata.h"
#include "vulkan/swapchain.h"

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT pMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        return func(instance, pMessenger, pAllocator);
    }
}

void cleanup(EngineData& enginedata) {

    vkDeviceWaitIdle(enginedata.device);

    cleanupSwapChain(enginedata.swapchain.swapchain, enginedata.swapchain.imageViews, enginedata.swapchainFramebuffers, enginedata.device);
    vkDestroyBuffer(enginedata.device, enginedata.vertexBuffer, nullptr);
    vkFreeMemory(enginedata.device, enginedata.vertexBufferMemory, nullptr);

    vkDestroyPipeline(enginedata.device, enginedata.graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(enginedata.device, enginedata.pipelineLayout, nullptr);

    vkDestroyRenderPass(enginedata.device, enginedata.renderPass, nullptr);

    for (size_t i = 0; i < enginedata.MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(enginedata.device, enginedata.syncObjects.imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(enginedata.device, enginedata.syncObjects.renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(enginedata.device, enginedata.syncObjects.inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(enginedata.device, enginedata.commandPool, nullptr);

    vkDestroyDevice(enginedata.device, nullptr);

#ifdef ENABLE_VALIDATION_LAYERS
    DestroyDebugUtilsMessengerEXT(enginedata.instance, enginedata.debugMessenger, nullptr);
#endif
    vkDestroySurfaceKHR(enginedata.instance, enginedata.window.surface, nullptr);
    vkDestroyInstance(enginedata.instance, nullptr);

    glfwDestroyWindow(enginedata.window.windowPointer);
    glfwTerminate();
}
