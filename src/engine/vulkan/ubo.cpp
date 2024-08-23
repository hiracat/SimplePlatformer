#include "../../utils/debugprint.h"
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

void createDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const uint32_t MAX_FRAMES_IN_FLIGHT) {
    VkDescriptorPoolSize poolSize{};
    poolSize.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.maxSets       = MAX_FRAMES_IN_FLIGHT;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes    = &poolSize;

    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        debugerror("failed to create descriptor pool");
    }
}
void createDescriptorSets(const VkDescriptorPool        pool,
                          std::vector<VkDescriptorSet>& descriptorSet,
                          const VkDescriptorSetLayout   layout,
                          uint32_t                      MAX_FRAMES_IN_FLIGHT) {
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, layout);
    VkDescriptorSetAllocateInfo        allocInfo{};

    allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool     = pool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts        = layouts.data();
}
