#include <chrono>
#include <vector>
#include <vulkan/vulkan.h>

#include <cstdint>
#include <vulkan/vulkan_core.h>

#include "../../utils/debugprint.h"
#include "../engine.h"
#include "buffers.h"
#include "commandobjects.h"
#include "swapchain.h"
#include "syncronization.h"

void drawFrame(EngineData& data, const std::vector<Model>& models) {

    vkWaitForFences(data.vulkanData.device, 1, &data.renderData.syncResources.inFlight[data.currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t currentImageIndex;
    VkResult result = vkAcquireNextImageKHR(data.vulkanData.device,
                                            data.renderData.swapchain.swapchain,
                                            UINT64_MAX,
                                            data.renderData.syncResources.imageAvailable[data.currentFrame],
                                            VK_NULL_HANDLE,
                                            &currentImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || data.windowData.framebufferResized) {
        data.windowData.framebufferResized = false;
        recreateSwapChain(
            data.vulkanData, data.renderData.pipelineResources.renderPass, &data.renderData.swapchain, &data.renderData.swapchainResources);

        cleanupSyncObjects(data.renderData.syncResources, data.vulkanData.device, data.MAX_FRAMES_IN_FLIGHT);
        createSyncObjects(data.renderData.syncResources, data.vulkanData.device, data.MAX_FRAMES_IN_FLIGHT);
        return;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        debugerror("unknown error while aquiring next image");
    }

    VkResult fenceStatus = vkGetFenceStatus(data.vulkanData.device, data.renderData.syncResources.inFlight[data.currentFrame]);
    // only reset fences when we know we are submitting work
    vkResetFences(data.vulkanData.device, 1, &data.renderData.syncResources.inFlight[data.currentFrame]);

    vkResetCommandBuffer(data.renderData.commandResources.buffers[data.currentFrame], 0);

    beginDrawing(data.renderData.commandResources.buffers[data.currentFrame],
                 data.renderData.swapchain.extent,
                 data.renderData.pipelineResources.renderPass,
                 data.renderData.swapchainResources.framebuffers[currentImageIndex],
                 data.renderData.pipelineResources.graphicsPipeline);

    updateUniformBuffers(data.currentFrame,
                         data.startTime,
                         data.renderData.swapchain.extent,
                         data.renderData.transformResources.uniformBuffers,
                         models[0].position,
                         0);
    updateUniformBuffers(data.currentFrame,
                         data.startTime,
                         data.renderData.swapchain.extent,
                         data.renderData.transformResources.uniformBuffers,
                         models[1].position,
                         1);

    for (uint32_t i = 0; i < models.size(); i++) {
        VkDeviceSize offsets[]     = {0};
        uint32_t     dynamicOffset = i * sizeof(MVPMatricies);

        vkCmdBindDescriptorSets(data.renderData.commandResources.buffers[data.currentFrame],
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                data.renderData.pipelineResources.pipelineLayout,
                                0,
                                1,
                                &data.renderData.descriptorResources.sets[data.currentFrame],
                                1,
                                &dynamicOffset);

        vkCmdBindVertexBuffers(data.renderData.commandResources.buffers[data.currentFrame], 0, 1, &models[i].vertexBuffer.buffer, offsets);
        vkCmdBindIndexBuffer(
            data.renderData.commandResources.buffers[data.currentFrame], models[i].indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(
            data.renderData.commandResources.buffers[data.currentFrame], static_cast<uint32_t>(models[i].indices.size()), 1, 0, 0, 0);
    }

    endDrawing(data.renderData.commandResources.buffers[data.currentFrame]);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore          waitSemaphores[] = {data.renderData.syncResources.imageAvailable[data.currentFrame]};
    VkPipelineStageFlags waitStages[]     = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount         = 1;
    submitInfo.pWaitSemaphores            = waitSemaphores;
    submitInfo.pWaitDstStageMask          = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &data.renderData.commandResources.buffers[data.currentFrame];

    VkSemaphore signalSemaphores[]  = {data.renderData.syncResources.renderFinished[data.currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;

    if (vkQueueSubmit(data.vulkanData.queues.graphics, 1, &submitInfo, data.renderData.syncResources.inFlight[data.currentFrame]) !=
        VK_SUCCESS) {
        debugerror("failed to submit draw command buffer");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = signalSemaphores;

    VkSwapchainKHR swapchains[] = {data.renderData.swapchain.swapchain};
    presentInfo.swapchainCount  = 1;
    presentInfo.pSwapchains     = swapchains;
    presentInfo.pImageIndices   = &currentImageIndex;

    result = vkQueuePresentKHR(data.vulkanData.queues.present, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        data.windowData.framebufferResized = false;
        recreateSwapChain(
            data.vulkanData, data.renderData.pipelineResources.renderPass, &data.renderData.swapchain, &data.renderData.swapchainResources);
    } else if (result != VK_SUCCESS) {
        debugerror("failed to present swap chain image!");
    }

    data.currentFrame = (data.currentFrame + 1) % data.MAX_FRAMES_IN_FLIGHT;
}
