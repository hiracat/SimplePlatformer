#include <vulkan/vulkan.h>

#include <cstdint>
#include <vulkan/vulkan_core.h>

#include "../../game/gamedata.h"
#include "../../utils/debugprint.h"
#include "../enginedata.h"
#include "commandobjects.h"
#include "swapchain.h"
#include "syncronization.h"

void drawFrame(Data& data, GameData& gamedata) {

    vkWaitForFences(data.device, 1, &data.syncResources.inFlightFences[data.currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(data.device,
                                            data.swapchain.swapchain,
                                            UINT64_MAX,
                                            data.syncResources.imageAvailableSemaphores[data.currentFrame],
                                            VK_NULL_HANDLE,
                                            &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || data.framebufferResized) {
        data.framebufferResized = false;
        recreateSwapChain(data.physicalDevice,
                          data.windowResources,
                          data.swapchain,
                          data.device,
                          data.pipelineResources.renderPass,
                          data.swapchainResources,
                          data.queueFamilyIndices);
        cleanupSyncObjects(data.syncResources, data.device, data.MAX_FRAMES_IN_FLIGHT);
        createSyncObjects(data.syncResources, data.device, data.MAX_FRAMES_IN_FLIGHT);
        return;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        debugerror("unknown error while aquiring next image");
    }

    VkResult fenceStatus = vkGetFenceStatus(data.device, data.syncResources.inFlightFences[data.currentFrame]);
    // debugnote("fence status: " << fenceStatus);
    // only reset fences when we know we are submitting work
    vkResetFences(data.device, 1, &data.syncResources.inFlightFences[data.currentFrame]);

    vkResetCommandBuffer(data.commandResources.commandBuffers[data.currentFrame], 0);

    recordCommandBuffer(data.commandResources.commandBuffers[data.currentFrame],
                        data.swapchain.extent,
                        data.pipelineResources.renderPass,
                        data.swapchainResources.swapchainFramebuffers[imageIndex],
                        data.pipelineResources.graphicsPipeline,
                        data.vertexBuffer.buffer,
                        data.indexBuffer.buffer,
                        gamedata.models[0].indices.size());

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore          waitSemaphores[] = {data.syncResources.imageAvailableSemaphores[data.currentFrame]};
    VkPipelineStageFlags waitStages[]     = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount         = 1;
    submitInfo.pWaitSemaphores            = waitSemaphores;
    submitInfo.pWaitDstStageMask          = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &data.commandResources.commandBuffers[data.currentFrame];

    VkSemaphore signalSemaphores[]  = {data.syncResources.renderFinishedSemaphores[data.currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;

    if (vkQueueSubmit(data.queues.graphicsQueue, 1, &submitInfo, data.syncResources.inFlightFences[data.currentFrame]) != VK_SUCCESS) {
        debugerror("failed to submit draw command buffer");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = signalSemaphores;

    VkSwapchainKHR swapchains[] = {data.swapchain.swapchain};
    presentInfo.swapchainCount  = 1;
    presentInfo.pSwapchains     = swapchains;
    presentInfo.pImageIndices   = &imageIndex;

    result = vkQueuePresentKHR(data.queues.presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        data.framebufferResized = false;
        recreateSwapChain(data.physicalDevice,
                          data.windowResources,
                          data.swapchain,
                          data.device,
                          data.pipelineResources.renderPass,
                          data.swapchainResources,
                          data.queueFamilyIndices);
    } else if (result != VK_SUCCESS) {
        debugerror("failed to present swap chain image!");
    }

    data.currentFrame = (data.currentFrame + 1) % data.MAX_FRAMES_IN_FLIGHT;
}
