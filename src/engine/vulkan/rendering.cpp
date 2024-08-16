#include <vulkan/vulkan.h>

#include <cstdint>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "../../utils/debugprint.h"
#include "../enginedata.h"
#include "commandobjects.h"
#include "swapchain.h"
#include "syncronization.h"

void drawFrame(Data& data) {

    vkWaitForFences(data.vulkanObjects.device, 1, &data.resources.syncObjects.inFlightFences[data.currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(data.vulkanObjects.device,
                                            data.renderingObjects.swapchain.swapchain,
                                            UINT64_MAX,
                                            data.resources.syncObjects.imageAvailableSemaphores[data.currentFrame],
                                            VK_NULL_HANDLE,
                                            &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || data.framebufferResized) {
        data.framebufferResized = false;
        recreateSwapChain(data.vulkanObjects.physicalDevice,
                          data.window,
                          data.renderingObjects.swapchain,
                          data.vulkanObjects.device,
                          data.renderingObjects.renderPass,
                          data.resources.swapchainFramebuffers,
                          data.resources.images,
                          data.resources.imageViews);
        cleanupSyncObjects(data.resources.syncObjects, data.vulkanObjects.device, data.resources.MAX_FRAMES_IN_FLIGHT);
        createSyncObjects(data.resources.syncObjects, data.vulkanObjects.device, data.resources.MAX_FRAMES_IN_FLIGHT);
        return;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        debugerror("unknown error while aquiring next image");
    }

    VkResult fenceStatus = vkGetFenceStatus(data.vulkanObjects.device, data.resources.syncObjects.inFlightFences[data.currentFrame]);
    // debugnote("fence status: " << fenceStatus);
    // only reset fences when we know we are submitting work
    vkResetFences(data.vulkanObjects.device, 1, &data.resources.syncObjects.inFlightFences[data.currentFrame]);

    vkResetCommandBuffer(data.resources.commandBuffers[data.currentFrame], 0);

    recordCommandBuffer(data.resources.commandBuffers[data.currentFrame],
                        data.renderingObjects.swapchain.extent,
                        data.renderingObjects.renderPass,
                        data.resources.swapchainFramebuffers[imageIndex],
                        data.renderingObjects.graphicsPipeline,
                        data.resources.vertexBuffer,
                        data.resources.indexBuffer,
                        data.resources.renderData.indices.size());

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore          waitSemaphores[] = {data.resources.syncObjects.imageAvailableSemaphores[data.currentFrame]};
    VkPipelineStageFlags waitStages[]     = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount         = 1;
    submitInfo.pWaitSemaphores            = waitSemaphores;
    submitInfo.pWaitDstStageMask          = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &data.resources.commandBuffers[data.currentFrame];

    VkSemaphore signalSemaphores[]  = {data.resources.syncObjects.renderFinishedSemaphores[data.currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;

    if (vkQueueSubmit(data.vulkanObjects.graphicsQueue, 1, &submitInfo, data.resources.syncObjects.inFlightFences[data.currentFrame]) !=
        VK_SUCCESS) {
        debugerror("failed to submit draw command buffer");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = signalSemaphores;

    VkSwapchainKHR swapchains[] = {data.renderingObjects.swapchain.swapchain};
    presentInfo.swapchainCount  = 1;
    presentInfo.pSwapchains     = swapchains;
    presentInfo.pImageIndices   = &imageIndex;

    result = vkQueuePresentKHR(data.vulkanObjects.presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        data.framebufferResized = false;
        recreateSwapChain(data.vulkanObjects.physicalDevice,
                          data.window,
                          data.renderingObjects.swapchain,
                          data.vulkanObjects.device,
                          data.renderingObjects.renderPass,
                          data.resources.swapchainFramebuffers,
                          data.resources.images,
                          data.resources.imageViews);
    } else if (result != VK_SUCCESS) {
        debugerror("failed to present swap chain image!");
    }

    data.currentFrame = (data.currentFrame + 1) % data.resources.MAX_FRAMES_IN_FLIGHT;
}
