#include <vulkan/vulkan.h>

#include <cstring>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "../vertex.h"
#include "physicaldevice.h"
#include "utils.h"

void createBuffer(const VkDevice         device,
                  const VkPhysicalDevice physicalDevice,
                  VkDeviceSize           size,
                  VkBufferCreateFlags    usage,
                  VkSharingMode          sharingMode,
                  VkBuffer&              vertexBuffer,
                  VkMemoryPropertyFlags  properties,
                  VkDeviceMemory&        vertexBufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size        = size;
    bufferInfo.usage       = usage;
    bufferInfo.sharingMode = sharingMode;
    if (vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, vertexBuffer, &memRequirements);
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize  = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);
    if (vkAllocateMemory(device, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }
    vkBindBufferMemory(device, vertexBuffer, vertexBufferMemory, 0);
}
void copyBuffer(VkBuffer&           srcBuffer,
                VkBuffer&           dstBuffer,
                VkDeviceSize        size,
                const VkCommandPool commandPool,
                const VkDevice      device,
                const VkQueue       transferQueue) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool        = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size      = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &commandBuffer;

    // using a fence would be faster, as multiple could be scheduled and more for the driver to optomize
    vkQueueSubmit(transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(transferQueue);

    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void createVertexBuffer(const std::vector<Vertex>& vertices,
                        const VkDevice             device,
                        const VkPhysicalDevice     physicalDevice,
                        VkBuffer&                  vertexBuffer,
                        VkDeviceMemory&            vertexBufferMemory,
                        const VkSurfaceKHR         surface,
                        const VkCommandPool        commandPool,
                        const VkQueue              transferQueue) {

    VkDeviceSize  bufferSize = sizeof(vertices[0]) * vertices.size();
    VkSharingMode queueSharingMode;
    { // set sharing mode
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
        if (indices.isSame()) {
            queueSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        } else {
            queueSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }
    }

    VkBuffer       stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(device,
                 physicalDevice,
                 bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 queueSharingMode,
                 stagingBuffer,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    createBuffer(device,
                 physicalDevice,
                 bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 queueSharingMode,
                 vertexBuffer,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 vertexBufferMemory);

    copyBuffer(stagingBuffer, vertexBuffer, bufferSize, commandPool, device, transferQueue);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}
