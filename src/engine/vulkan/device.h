#pragma once
#include "../engine.h"

void createDevice(const VulkanData& vulkanData, VkDevice* device, Queues* queues);
bool checkExtensionSupported(const std::vector<VkExtensionProperties>& supportedExtensionProperties, const char* extensionName);
std::vector<const char*> getRequredExtensions();
void                     setupDebugMessanger(const VulkanData& vulkanData, VkDebugUtilsMessengerEXT* debugMessenger);
void                     populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* createInfo);
