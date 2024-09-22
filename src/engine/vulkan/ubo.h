#include "../engine.h"
#include <vector>
#include <vulkan/vulkan_core.h>

void createDescriptorPool(const VkDevice device, const uint32_t MAX_FRAMES_IN_FLIGHT, VkDescriptorPool* descriptorPool);
void createDescriptorSets(const RendererData& data, const uint32_t MAX_FRAMES_IN_FLIGHT, std::vector<VkDescriptorSet>& descriptorSets);
