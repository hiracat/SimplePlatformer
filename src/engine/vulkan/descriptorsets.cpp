#include <vulkan/vulkan_core.h>

#include "../../utils/debugprint.h"

void CreateDescriptorSetLayout(const VkDevice device, VkDescriptorSetLayout& layout) {
    VkDescriptorSetLayoutBinding mvpLayoutBinding{};
    mvpLayoutBinding.binding            = 0;
    mvpLayoutBinding.descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    mvpLayoutBinding.descriptorCount    = 1;
    mvpLayoutBinding.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
    mvpLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};

    layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.pBindings    = &mvpLayoutBinding;
    layoutInfo.bindingCount = 1;

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &layout) != VK_SUCCESS) {
        debugerror("failed to create descriptor set layout");
    }
}
