#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

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

void cleanup(AppData& appdata) {
    vkDeviceWaitIdle(appdata.device);
    vkDestroySemaphore(appdata.device, appdata.syncObjects.imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(appdata.device, appdata.syncObjects.renderFinishedSemaphore, nullptr);
    vkDestroyFence(appdata.device, appdata.syncObjects.inFlightFence, nullptr);

    vkDestroyCommandPool(appdata.device, appdata.commandPool, nullptr);
    for (auto framebuffer : appdata.swapchainFramebuffers) {
        vkDestroyFramebuffer(appdata.device, framebuffer, nullptr);
    }

    vkDestroyPipeline(appdata.device, appdata.graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(appdata.device, appdata.pipelineLayout, nullptr);
    vkDestroyRenderPass(appdata.device, appdata.renderPass, nullptr);
    for (auto& imageView : appdata.swapchain.imageViews) {
        vkDestroyImageView(appdata.device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(appdata.device, appdata.swapchain.swapchain, nullptr);

    vkDestroyDevice(appdata.device, nullptr);
    vkDestroySurfaceKHR(appdata.instance, appdata.window.surface, nullptr);

#ifdef ENABLE_VALIDATION_LAYERS
    DestroyDebugUtilsMessengerEXT(appdata.instance, appdata.debugMessenger, nullptr);
#endif
    vkDestroyInstance(appdata.instance, nullptr);

    glfwDestroyWindow(appdata.window.windowPointer);
    glfwTerminate();
}
