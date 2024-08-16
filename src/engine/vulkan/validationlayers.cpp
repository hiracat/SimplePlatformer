#include <vulkan/vulkan.h>

#include <cstring>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "../../utils/debugprint.h"

// this function is not automatically loaded so it needs to be manually loaded
VkResult CreateDebugUtilsMessengerEXT(VkInstance                                instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks*              pAllocator,
                                      VkDebugUtilsMessengerEXT*                 pMessenger) {
    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
                                             VkDebugUtilsMessageTypeFlagsEXT             messageType,
                                             const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                             void*                                       pUserData) {

    debugunknown("Validation layer says: " << pCallbackData->pMessage);
    return VK_FALSE;
}

void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                  .pNext = nullptr,
                  .flags = 0,

                  .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,

                  .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,

                  .pfnUserCallback = debugCallback,

                  .pUserData = nullptr};
}

void setupDebugMessanger(VkInstance instance, VkDebugUtilsMessengerEXT* debugMessenger) {

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    PopulateDebugMessengerCreateInfo(createInfo);

    VkResult result;

    result = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, debugMessenger);
    if (result != VK_SUCCESS) {
        debugerror("failed to create debug messenger");
    }
}

bool checkValidationLayerSupported(const std::vector<const char*>& validationLayers) {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;
        for (const VkLayerProperties& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            debugerror("requested layer " << layerName << " not available");
            return false;
        }
    }

    return true;
}
