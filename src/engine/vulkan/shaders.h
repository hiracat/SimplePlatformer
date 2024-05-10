#include <vector>
#include <vulkan/vulkan_core.h>

VkShaderModule createShaderModule(const VkDevice device, const std::vector<char>& code);
