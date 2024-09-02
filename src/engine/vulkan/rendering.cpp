#include <chrono>
#include <vulkan/vulkan.h>

#include <cstdint>
#include <vulkan/vulkan_core.h>

#include "../../game/gamedata.h"
#include "../../utils/debugprint.h"
#include "../enginedata.h"
#include "buffers.h"
#include "commandobjects.h"
#include "swapchain.h"
#include "syncronization.h"

void drawFrame(Data& data, GameData& gamedata, std::chrono::high_resolution_clock::time_point startTime) {

    vkWaitForFences(data.device, 1, &data.syncResources.inFlight[data.currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t currentImageIndex;
    VkResult result = vkAcquireNextImageKHR(data.device,
                                            data.swapchain.swapchain,
                                            UINT64_MAX,
                                            data.syncResources.imageAvailable[data.currentFrame],
                                            VK_NULL_HANDLE,
                                            &currentImageIndex);

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

    VkResult fenceStatus = vkGetFenceStatus(data.device, data.syncResources.inFlight[data.currentFrame]);
    // only reset fences when we know we are submitting work
    vkResetFences(data.device, 1, &data.syncResources.inFlight[data.currentFrame]);

    vkResetCommandBuffer(data.commandResources.buffers[data.currentFrame], 0);
    beginDrawing(data.commandResources.buffers[data.currentFrame],
                 data.swapchain.extent,
                 data.pipelineResources.renderPass,
                 data.swapchainResources.framebuffers[currentImageIndex],
                 data.pipelineResources.graphicsPipeline);

    VkDeviceSize offsets[] = {0};
    vkCmdBindDescriptorSets(data.commandResources.buffers[data.currentFrame],
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            data.pipelineResources.pipelineLayout,
                            0,
                            1,
                            &data.descriptorResources.sets[data.currentFrame],
                            0,
                            nullptr);

    updateUniformBuffers(
        data.currentFrame, startTime, data.swapchain.extent, data.transformResources.uniformBuffers, gamedata.models[0].position);

    vkCmdBindVertexBuffers(data.commandResources.buffers[data.currentFrame], 0, 1, &gamedata.models[0].vertexBuffer.buffer, offsets);
    vkCmdBindIndexBuffer(data.commandResources.buffers[data.currentFrame], gamedata.models[0].indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(
        data.commandResources.buffers[data.currentFrame], static_cast<uint32_t>(gamedata.models[0].indices.size()), 1, 0, 0, 0);

    updateUniformBuffers(
        data.currentFrame, startTime, data.swapchain.extent, data.transformResources.uniformBuffers, gamedata.models[1].position);
    vkCmdBindVertexBuffers(data.commandResources.buffers[data.currentFrame], 0, 1, &gamedata.models[1].vertexBuffer.buffer, offsets);
    vkCmdBindIndexBuffer(data.commandResources.buffers[data.currentFrame], gamedata.models[1].indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(
        data.commandResources.buffers[data.currentFrame], static_cast<uint32_t>(gamedata.models[1].indices.size()), 1, 0, 0, 0);

    endDrawing(data.commandResources.buffers[data.currentFrame]);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore          waitSemaphores[] = {data.syncResources.imageAvailable[data.currentFrame]};
    VkPipelineStageFlags waitStages[]     = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount         = 1;
    submitInfo.pWaitSemaphores            = waitSemaphores;
    submitInfo.pWaitDstStageMask          = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &data.commandResources.buffers[data.currentFrame];

    VkSemaphore signalSemaphores[]  = {data.syncResources.renderFinished[data.currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;

    if (vkQueueSubmit(data.queues.graphics, 1, &submitInfo, data.syncResources.inFlight[data.currentFrame]) != VK_SUCCESS) {
        debugerror("failed to submit draw command buffer");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = signalSemaphores;

    VkSwapchainKHR swapchains[] = {data.swapchain.swapchain};
    presentInfo.swapchainCount  = 1;
    presentInfo.pSwapchains     = swapchains;
    presentInfo.pImageIndices   = &currentImageIndex;

    result = vkQueuePresentKHR(data.queues.present, &presentInfo);

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
