#include <vulkan/vulkan.h>

#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "../../utils/debugprint.h"
#include "physicaldevice.h"

void recordCommandBuffer(VkCommandBuffer     commandBuffer,
                         const VkExtent2D&   swapChainExtent,
                         const VkRenderPass& renderPass,
                         VkFramebuffer&      frameBuffer,
                         const VkPipeline&   graphicsPipeline,
                         const VkBuffer      vertexBuffer,
                         const VkBuffer      indexBuffer,
                         const uint32_t      indicesCount) {

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags            = 0;
    beginInfo.pNext            = nullptr;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        debugerror("failed to being command buffer");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass  = renderPass;
    renderPassInfo.framebuffer = frameBuffer;

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChainExtent;

    VkClearValue clearColor        = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues    = &clearColor;

    // inline if primary command buffer, other option if secondary
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    VkViewport viewport{};
    viewport.x        = 0.0f;
    viewport.y        = 0.0f;
    viewport.width    = static_cast<float>(swapChainExtent.width);
    viewport.height   = static_cast<float>(swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    VkBuffer     vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[]       = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indicesCount), 1, 0, 0, 0);
    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        debugerror("failed to record command buffer!");
    }
}

void createCommandBuffers(const VkDevice& device, CommandResources& resources, const uint32_t maxFramesInFlight) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool        = resources.commandPool;
    allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = maxFramesInFlight;

    resources.commandBuffers.resize(maxFramesInFlight);

    if (vkAllocateCommandBuffers(device, &allocInfo, resources.commandBuffers.data())) {
        debugerror("failed to allocate command buffers");
    }
}

void createCommandPool(const VkPhysicalDevice& physicalDevice,
                       const uint32_t          queueFamilyIndex,
                       const VkDevice&         device,
                       const VkSurfaceKHR&     surface,
                       VkCommandPool&          commandPool) {
    QueueFamilyIndices      queueFamilyIndices = findQueueFamilies(physicalDevice, surface);
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.presentFamily.value();
    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        debugerror("failed to create command pool!");
    }
}
