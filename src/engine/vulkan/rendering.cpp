#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "commandobjects.h"
#include "syncronization.h"

void drawFrame(const VkDevice&                   device,
               const SyncObjects&                syncObjects,
               const VkSwapchainKHR&             swapchain,
               const VkCommandBuffer&            commandBuffer,
               const VkExtent2D&                 swapchainExtent,
               const VkRenderPass&               renderPass,
               const std::vector<VkFramebuffer>& swapchainFrameBuffers,
               const VkPipeline&                 graphicsPipeline,
               const VkQueue&                    graphicsQueue,
               const VkQueue&                    presentQueue) {

    vkWaitForFences(device, 1, &syncObjects.inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(device, 1, &syncObjects.inFlightFence);
    uint32_t imageIndex;
    vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, syncObjects.imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
    vkResetCommandBuffer(commandBuffer, 0);
    recordCommandBuffer(commandBuffer, swapchainExtent, renderPass, swapchainFrameBuffers[imageIndex], graphicsPipeline);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore          waitSemaphores[] = {syncObjects.imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[]     = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores    = waitSemaphores;
    submitInfo.pWaitDstStageMask  = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &commandBuffer;

    VkSemaphore signalSemaphores[]  = {syncObjects.renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, syncObjects.inFlightFence) != VK_SUCCESS) { // errors on this line
        throw std::runtime_error("failed to submit draw command buffer");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = signalSemaphores;

    VkSwapchainKHR swapchains[] = {swapchain};
    presentInfo.swapchainCount  = 1;
    presentInfo.pSwapchains     = swapchains;
    presentInfo.pImageIndices   = &imageIndex;

    vkQueuePresentKHR(presentQueue, &presentInfo);
}
