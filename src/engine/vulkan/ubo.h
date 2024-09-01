#include "../enginedata.h"
#include <vector>
#include <vulkan/vulkan_core.h>

void createDescriptorPool(VkDevice device, VkDescriptorPool& descriptorPool, const uint32_t MAX_FRAMES_IN_FLIGHT);
void createDescriptorSets(VkDevice                      device,
                          std::vector<UniformBuffer>&   buffers,
                          const VkDescriptorPool        pool,
                          std::vector<VkDescriptorSet>& descriptorSets,
                          const VkDescriptorSetLayout   layout,
                          uint32_t                      MAX_FRAMES_IN_FLIGHT);
