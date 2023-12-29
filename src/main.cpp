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

#ifndef NDEBUG
#define ENABLE_VALIDATION_LAYERS
#endif

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                             VkDebugUtilsMessageTypeFlagsEXT messageType,
                                             const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
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
    uint32_t glfwExtensionCount;
    const char** glfwExtensionNames = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    uint32_t requiredExtensionCount;
    std::vector<const char*> requiredExtensions(glfwExtensionNames, glfwExtensionNames + glfwExtensionCount);

#ifdef ENABLE_VALIDATION_LAYERS
    requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    return requiredExtensions;
}

void createVkInstance(VkInstance* instance, std::vector<const char*> validationLayers) {

    VkApplicationInfo appInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = "simple platformer",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "no engine",
        .engineVersion = VK_MAKE_VERSION(0, 0, 0),
        .apiVersion = VK_MAKE_VERSION(1, 3, 269),
    };

    std::vector<const char*> requiredExtensions = getRequredExtensions();

    VkInstanceCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &appInfo,
        // conditionally enable the vaildation layers later
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
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
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
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
        std::cout << "result is: " << string_VkResult(result) << std::endl;
        throw std::runtime_error("failed to create instance!");
    }
}

void setupDebugMessanger() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo{
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = nullptr,
        .flags = 0,

        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,

        .messageType = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,

        .pfnUserCallback = debugCallback,

        .pUserData = nullptr};
}

struct Window {
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 800;
    GLFWwindow* windowPointer;
};

struct AppData {
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    Window window{};
    VkInstance instance{};
    VkDebugUtilsMessengerEXT debugMessenger;
};

void cleanup(AppData appdata) {
    vkDestroyInstance(appdata.instance, nullptr);

    glfwDestroyWindow(appdata.window.windowPointer);
    glfwTerminate();
}

int main() {
    AppData appdata;
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwInit();

    appdata.window.windowPointer = glfwCreateWindow(appdata.window.WIDTH, appdata.window.HEIGHT, "bruh", nullptr, nullptr);
    if (!appdata.window.windowPointer) {
        glfwTerminate();
        throw std::runtime_error("failed to create window");
    }
    createVkInstance(&appdata.instance, appdata.validationLayers);
#ifdef ENABLE_VALIDATION_LAYERS
    setupDebugMessanger();
#endif

    while (!glfwWindowShouldClose(appdata.window.windowPointer)) {
        glfwPollEvents();
    }

    cleanup(appdata);
}
