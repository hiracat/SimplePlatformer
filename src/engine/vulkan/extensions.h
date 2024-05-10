#include <vector>
#include <vulkan/vulkan_core.h>

bool checkExtensionSupported(const std::vector<VkExtensionProperties>& supportedExtensionProperties, const char* extensionName);
std::vector<const char*> getRequredExtensions();
