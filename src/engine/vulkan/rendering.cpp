#include <cstdint>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "../window.h"
#include "commandobjects.h"
#include "swapchain.h"
#include "syncronization.h"

void drawFrame(const VkDevice                      device,
               const SyncObjects&                  syncObjects,
               Swapchain&                          swapchain,
               const std::vector<VkCommandBuffer>& commandBuffers,
               const VkExtent2D&                   swapchainExtent,
               const VkRenderPass                  renderPass,
               std::vector<VkFramebuffer>&         swapchainFrameBuffers,
               const VkPipeline                    graphicsPipeline,
               const VkQueue                       graphicsQueue,
               const VkQueue                       presentQueue,
               uint32_t&                           currentFrame,
               const uint32_t                      maxFramesInFlight,
               const Window&                       window,
               const VkPhysicalDevice              physicalDevice,
               const VkSurfaceKHR                  surface,
               bool&                               frameBufferResized,
               const VkBuffer                      vertexBuffer,
               const uint32_t                      verticesCount) {

    vkWaitForFences(device, 1, &syncObjects.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(
        device, swapchain.swapchain, UINT64_MAX, syncObjects.imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || frameBufferResized) {
        frameBufferResized = false;
        recreateSwapChain(physicalDevice, surface, window, swapchain, device, renderPass, swapchainFrameBuffers);
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    // only reset fences when we know we are submitting work
    vkResetFences(device, 1, &syncObjects.inFlightFences[currentFrame]);

    vkResetCommandBuffer(commandBuffers[currentFrame], 0);
    recordCommandBuffer(commandBuffers[currentFrame],
                        swapchainExtent,
                        renderPass,
                        swapchainFrameBuffers[imageIndex],
                        graphicsPipeline,
                        vertexBuffer,
                        verticesCount);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore          waitSemaphores[] = {syncObjects.imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[]     = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount         = 1;
    submitInfo.pWaitSemaphores            = waitSemaphores;
    submitInfo.pWaitDstStageMask          = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[]  = {syncObjects.renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, syncObjects.inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = signalSemaphores;

    VkSwapchainKHR swapchains[] = {swapchain.swapchain};
    presentInfo.swapchainCount  = 1;
    presentInfo.pSwapchains     = swapchains;
    presentInfo.pImageIndices   = &imageIndex;

    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        frameBufferResized = false;
        recreateSwapChain(physicalDevice, surface, window, swapchain, device, renderPass, swapchainFrameBuffers);
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % maxFramesInFlight;
}
