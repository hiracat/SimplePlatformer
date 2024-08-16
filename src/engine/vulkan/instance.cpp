#include <vulkan/vulkan.h>

#include <vector>

#include "../../compilesettings.h"
#include "../../utils/debugprint.h"
#include "extensions.h"
#include "validationlayers.h"

void createVkInstance(VkInstance& instance, const std::vector<const char*>& validationLayers) {

    VkApplicationInfo appInfo{
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext              = nullptr,
        .pApplicationName   = "simple platformer",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName        = "no engine",
        .engineVersion      = VK_MAKE_VERSION(0, 0, 0),
        .apiVersion         = VK_MAKE_VERSION(1, 3, 269),
    };

    std::vector<const char*> requiredExtensions = getRequredExtensions();

    VkInstanceCreateInfo createInfo{
        .sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext            = nullptr,
        .flags            = 0,
        .pApplicationInfo = &appInfo,
        // conditionally enable the vaildation layers later
        .enabledLayerCount       = 0,
        .ppEnabledLayerNames     = nullptr,
        .enabledExtensionCount   = static_cast<uint32_t>(requiredExtensions.size()),
        .ppEnabledExtensionNames = requiredExtensions.data(),
    };

    uint32_t extensionCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> supportedExtensionProperties(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, supportedExtensionProperties.data());

#ifndef NDEBUG
    for (const VkExtensionProperties& extension : supportedExtensionProperties) {
        debugnote("Available Extension: " << extension.extensionName);
    }
#endif

#ifdef ENABLE_VALIDATION_LAYERS
    if (!checkValidationLayerSupported(validationLayers)) {
        debugerror("validation layers not supported");
    };

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    PopulateDebugMessengerCreateInfo(debugCreateInfo);

    createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
    createInfo.pNext               = &debugCreateInfo;
#endif

    for (uint32_t i = 0; i < createInfo.enabledExtensionCount; i++) {
        if (!checkExtensionSupported(supportedExtensionProperties, createInfo.ppEnabledExtensionNames[i])) {
            debugerror("an extensions was requrested but not available called: " << createInfo.ppEnabledExtensionNames[i]);
        }
    }

    VkResult result;

    if ((result = vkCreateInstance(&createInfo, nullptr, &instance)) != VK_SUCCESS) {
        debugerror("failed to create instance!");
    }
}
