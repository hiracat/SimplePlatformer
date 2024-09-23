#include "../engine.h"
#include <vector>
#include <vulkan/vulkan_core.h>

void createDescriptorPool(const VulkanData& vulkanData, VkDescriptorPool* descriptorPool);
void createDescriptorSets(const RendererData& renderData, std::vector<VkDescriptorSet>* descriptorSets);
