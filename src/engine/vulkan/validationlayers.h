#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

void setupDebugMessanger(VkInstance instance, VkDebugUtilsMessengerEXT* debugMessenger);
bool checkValidationLayerSupported(const std::vector<const char*>& validationLayers);
void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
