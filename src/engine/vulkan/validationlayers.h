#pragma once
#include <vector>
#include <vulkan/vulkan.h>

void setupDebugMessanger(VkInstance instance, VkDebugUtilsMessengerEXT* debugMessenger);
bool checkValidationLayerSupported(const std::vector<const char*>& validationLayers);
void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
