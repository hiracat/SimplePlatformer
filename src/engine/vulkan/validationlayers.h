#pragma once
#include "../engine.h"
#include <vulkan/vulkan.h>

void setupDebugMessanger(const VulkanData& vulkanData, VkDebugUtilsMessengerEXT* debugMessenger);
void setupDebugMessanger(const VulkanData& vulkanData, VkDebugUtilsMessengerEXT* debugMessenger);
void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
