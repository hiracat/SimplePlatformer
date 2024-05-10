#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "../compilesettings.h"
#include "../game/main.h"

void DestroyDebugUtilsMessengerEXT(VkInstance                   instance,
                                   VkDebugUtilsMessengerEXT     pMessenger,
                                   const VkAllocationCallbacks* pAllocator) {
    auto func =
        reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        return func(instance, pMessenger, pAllocator);
    }
}

void cleanup(EngineData& enginedata) {
    vkDeviceWaitIdle(enginedata.device);
    vkDestroySemaphore(enginedata.device, enginedata.syncObjects.imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(enginedata.device, enginedata.syncObjects.renderFinishedSemaphore, nullptr);
    vkDestroyFence(enginedata.device, enginedata.syncObjects.inFlightFence, nullptr);

    vkDestroyCommandPool(enginedata.device, enginedata.commandPool, nullptr);
    for (auto framebuffer : enginedata.swapchainFramebuffers) {
        vkDestroyFramebuffer(enginedata.device, framebuffer, nullptr);
    }

    vkDestroyPipeline(enginedata.device, enginedata.graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(enginedata.device, enginedata.pipelineLayout, nullptr);
    vkDestroyRenderPass(enginedata.device, enginedata.renderPass, nullptr);
    for (auto& imageView : enginedata.swapchain.imageViews) {
        vkDestroyImageView(enginedata.device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(enginedata.device, enginedata.swapchain.swapchain, nullptr);

    vkDestroyDevice(enginedata.device, nullptr);
    vkDestroySurfaceKHR(enginedata.instance, enginedata.window.surface, nullptr);

#ifdef ENABLE_VALIDATION_LAYERS
    DestroyDebugUtilsMessengerEXT(enginedata.instance, enginedata.debugMessenger, nullptr);
#endif
    vkDestroyInstance(enginedata.instance, nullptr);

    glfwDestroyWindow(enginedata.window.windowPointer);
    glfwTerminate();
}
