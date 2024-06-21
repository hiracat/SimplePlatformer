#pragma once
#include <vulkan/vulkan.h>

#include <vector>

bool checkExtensionSupported(const std::vector<VkExtensionProperties>& supportedExtensionProperties, const char* extensionName);
std::vector<const char*> getRequredExtensions();
