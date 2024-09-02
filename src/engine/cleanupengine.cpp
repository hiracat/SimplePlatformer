#include <cstddef>
#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "../compilesettings.h"
#include "../game/gamedata.h"
#include "enginedata.h"
#include "vulkan/swapchain.h"
#include "vulkan/syncronization.h"

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT pMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        return func(instance, pMessenger, pAllocator);
    }
}

void cleanup(Data& data, GameData& gameData) {

    vkDeviceWaitIdle(data.device);

    cleanupSwapChain(data.swapchain.swapchain, data.swapchainResources.imageViews, data.swapchainResources.framebuffers, data.device);

    for (size_t i = 0; i < data.MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(data.device, data.transformResources.uniformBuffers[i].buffer.buffer, nullptr);
        vkFreeMemory(data.device, data.transformResources.uniformBuffers[i].buffer.memory, nullptr);
    }
    vkDestroyDescriptorPool(data.device, data.descriptorResources.pool, nullptr);
    vkDestroyDescriptorSetLayout(data.device, data.transformResources.descriptorSetLayout, nullptr);

    for (size_t i = 0; i < gameData.models.size(); i++) {
        vkDestroyBuffer(data.device, gameData.models[i].vertexBuffer.buffer, nullptr);
        vkFreeMemory(data.device, gameData.models[i].vertexBuffer.memory, nullptr);

        vkDestroyBuffer(data.device, gameData.models[i].indexBuffer.buffer, nullptr);
        vkFreeMemory(data.device, gameData.models[i].indexBuffer.memory, nullptr);
    }

    vkDestroyPipeline(data.device, data.pipelineResources.graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(data.device, data.pipelineResources.pipelineLayout, nullptr);

    vkDestroyRenderPass(data.device, data.pipelineResources.renderPass, nullptr);

    cleanupSyncObjects(data.syncResources, data.device, data.MAX_FRAMES_IN_FLIGHT);

    vkDestroyCommandPool(data.device, data.commandResources.pool, nullptr);

    vkDestroyDevice(data.device, nullptr);

#ifdef ENABLE_VALIDATION_LAYERS
    DestroyDebugUtilsMessengerEXT(data.instance, data.instanceResources.debugMessenger, nullptr);
#endif
    vkDestroySurfaceKHR(data.instance, data.windowResources.surface, nullptr);
    vkDestroyInstance(data.instance, nullptr);

    glfwDestroyWindow(data.windowResources.pointer);
    glfwTerminate();
}
