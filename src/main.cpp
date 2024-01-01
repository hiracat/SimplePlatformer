#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <GLFW/glfw3.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "ansiescapecodes.h"
#include "physicaldevice.h"
#include "window.h"

#ifndef NDEBUG
#define ENABLE_VALIDATION_LAYERS
#endif

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
                                             VkDebugUtilsMessageTypeFlagsEXT             messageType,
                                             const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                             void*                                       pUserData) {

    std::cerr << ANSI_COLOR_RED << "validation layer says: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

bool checkExtensionSupported(const std::vector<VkExtensionProperties>& supportedExtensionProperties, const char* extensionName) {
    bool supported = false;
    for (const VkExtensionProperties& extension : supportedExtensionProperties) {
        if (strcmp(extensionName, extension.extensionName) == 0) {
            supported = true;
            break;
        }
    }
    return supported;
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
            std::cout << ANSI_COLOR_RED << "requested layer: " << layerName << " not available" << ANSI_RESET << std::endl;
            return false;
        }
    }

    return true;
}

std::vector<const char*> getRequredExtensions() {
    uint32_t     glfwExtensionCount;
    const char** glfwExtensionNames = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    uint32_t                 requiredExtensionCount;
    std::vector<const char*> requiredExtensions(glfwExtensionNames, glfwExtensionNames + glfwExtensionCount);

#ifdef ENABLE_VALIDATION_LAYERS
    requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    return requiredExtensions;
}

void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                  .pNext = nullptr,
                  .flags = 0,

                  .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,

                  .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,

                  .pfnUserCallback = debugCallback,

                  .pUserData = nullptr};
}

void createVkInstance(VkInstance* instance, std::vector<const char*> validationLayers) {

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
        std::cout << "Available Extension: " << extension.extensionName << std::endl;
    }
#endif

#ifdef ENABLE_VALIDATION_LAYERS
    if (!checkValidationLayerSupported(validationLayers)) {
        throw std::runtime_error("validation layers not supported");
    };

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    PopulateDebugMessengerCreateInfo(debugCreateInfo);

    createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
    createInfo.pNext               = &debugCreateInfo;
#endif

    for (uint32_t i = 0; i < createInfo.enabledExtensionCount; i++) {
        if (!checkExtensionSupported(supportedExtensionProperties, createInfo.ppEnabledExtensionNames[i])) {
            std::string message("an extension was requested but not available called: ");
            message.append(createInfo.ppEnabledExtensionNames[i]);
            throw std::runtime_error(message);
        }
    }

    VkResult result;

    if ((result = vkCreateInstance(&createInfo, nullptr, instance)) != VK_SUCCESS) {
        std::cerr << "result is: " << string_VkResult(result) << std::endl;
        throw std::runtime_error("failed to create instance!");
    }
}

struct AppData {
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    VkDebugUtilsMessengerEXT       debugMessenger;
    Window                         window{};
    VkInstance                     instance{};
    VkDevice                       device;
    VkPhysicalDevice               physicalDevice = VK_NULL_HANDLE;
    VkQueue                        graphicsQueue;
};

VkResult CreateDebugUtilsMessengerEXT(VkInstance                                instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks*              pAllocator,
                                      VkDebugUtilsMessengerEXT*                 pMessenger) {
    auto func =
        reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
void DestroyDebugUtilsMessengerEXT(VkInstance                   instance,
                                   VkDebugUtilsMessengerEXT     pMessenger,
                                   const VkAllocationCallbacks* pAllocator) {
    auto func =
        reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        return func(instance, pMessenger, pAllocator);
    }
}

void setupDebugMessanger(VkInstance& instance, VkDebugUtilsMessengerEXT* debugMessenger) {

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    PopulateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messeneger");
    }
}

void cleanup(AppData appdata) {
    vkDestroyDevice(appdata.device, nullptr);
#ifdef ENABLE_VALIDATION_LAYERS
    DestroyDebugUtilsMessengerEXT(appdata.instance, appdata.debugMessenger, nullptr);
#endif
    vkDestroyInstance(appdata.instance, nullptr);

    glfwDestroyWindow(appdata.window.windowPointer);
    glfwTerminate();
}
void createLogicalDevice(VkDevice                        device,
                         VkPhysicalDevice                physicalDevice,
                         const std::vector<const char*>& validationLayers,
                         VkQueue                         graphicsQueue) {

    std::cout << "before findQueueFAmilies in createLogicalDevice" << std::endl;
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
    std::cout << "after findQueueFAmilies in createLogicalDevice" << std::endl;
    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueCreateInfo{
        .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = indices.graphicsFamily.value(),
        .queueCount       = 1,
        .pQueuePriorities = &queuePriority,
    };

    VkPhysicalDeviceFeatures2 deviceFeatures{};

    std::cout << "###########defined all the stuff idk" << std::endl;
    VkDeviceCreateInfo createInfo{
        .sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos    = &queueCreateInfo,

        .enabledExtensionCount = 0,
        .pEnabledFeatures      = &deviceFeatures.features,
    };

    std::cout << "###########defined all the stuff idk" << std::endl;

#ifdef ENABLE_VALIDATION_LAYERS
    createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
#else
    createInfo.enabledLayerCount = 0;
#endif
    std::cout << "############defined all the stuff idk" << std::endl;

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device");
    }
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
}

int main() {

    AppData appdata;
    initializeWindow(appdata.window);
    std::cout << ANSI_COLOR_GREEN << " init window" << ANSI_RESET << std::endl;
    createVkInstance(&appdata.instance, appdata.validationLayers);
    std::cout << "make vk instance" << std::endl;
#ifdef ENABLE_VALIDATION_LAYERS
    setupDebugMessanger(appdata.instance, &appdata.debugMessenger);
    std::cout << "setup debug messenger" << std::endl;
#endif
    pickPhysicalDevice(appdata.instance, appdata.physicalDevice);
    std::cout << "pick physical device" << std::endl;
    createLogicalDevice(appdata.device, appdata.physicalDevice, appdata.validationLayers, appdata.graphicsQueue);
    std::cout << "created logical device" << std::endl;

    while (!glfwWindowShouldClose(appdata.window.windowPointer)) {
        glfwPollEvents();
    }

    std::cout << "window loop ended" << std::endl;
    cleanup(appdata);
    std::cout << "all cleaned up" << std::endl;
}
