#include "../../utils/debugprint.h"
#include "../engine.h"
#include <cstddef>
#include <cstdint>
#include <glm/fwd.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

void createDescriptorPool(const VulkanData& vulkanData, VkDescriptorPool* descriptorPool) {
    VkDescriptorPoolSize poolSize{};
    poolSize.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    poolSize.descriptorCount = static_cast<uint32_t>(vulkanData.MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.maxSets       = vulkanData.MAX_FRAMES_IN_FLIGHT;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes    = &poolSize;

    if (vkCreateDescriptorPool(vulkanData.device, &poolInfo, nullptr, descriptorPool) != VK_SUCCESS) {
        debugerror("failed to create descriptor pool");
    }
}
void createDescriptorSets(const RendererData& renderData, std::vector<VkDescriptorSet>* descriptorSets) {

    std::vector<VkDescriptorSetLayout> layouts(renderData.MAX_FRAMES_IN_FLIGHT, renderData.transformResources.descriptorSetLayout);
    VkDescriptorSetAllocateInfo        allocInfo{};

    allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool     = renderData.descriptorResources.pool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(renderData.MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts        = layouts.data();

    descriptorSets->resize(renderData.MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(renderData.device, &allocInfo, descriptorSets->data()) != VK_SUCCESS) {
        debugerror("failed to allocate descriptor sets");
    }
    for (size_t i = 0; i < renderData.MAX_FRAMES_IN_FLIGHT; ++i) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = renderData.transformResources.uniformBuffers[i].buffer.buffer;
        bufferInfo.offset = 0;
        bufferInfo.range  = sizeof(MVPMatricies);

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet          = (*descriptorSets)[i];
        descriptorWrite.dstBinding      = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo     = &bufferInfo;
        vkUpdateDescriptorSets(renderData.device, 1, &descriptorWrite, 0, nullptr);
    }
}
