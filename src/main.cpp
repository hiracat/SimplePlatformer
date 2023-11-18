#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <GLFW/glfw3.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan.h>

#include "ansiescapecodes.h"

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

void createVkInstance(VkInstance* instance) {

    VkApplicationInfo appInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = "simple platformer",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "no engine",
        .engineVersion = VK_MAKE_VERSION(0, 0, 0),
        .apiVersion = VK_MAKE_VERSION(1, 3, 269),
    };

    VkInstanceCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = nullptr,
    };

    createInfo.ppEnabledExtensionNames = glfwGetRequiredInstanceExtensions(&createInfo.enabledExtensionCount);

    uint32_t supportedExtensionCount;
    std::vector<VkExtensionProperties> supportedExtensionProperties;

    vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, nullptr);
    supportedExtensionProperties.resize(supportedExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, supportedExtensionProperties.data());

    for (const VkExtensionProperties& extension : supportedExtensionProperties) {
        std::cout << "Available Extension: " << extension.extensionName << std::endl;
    }

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

struct AppData {
    GLFWwindow* window;
    VkInstance instance;
};

void cleanup(AppData appdata) {
    vkDestroyInstance(appdata.instance, nullptr);

    glfwDestroyWindow(appdata.window);
    glfwTerminate();
}

int main() {
    AppData appdata;
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwInit();

    appdata.window = glfwCreateWindow(800, 600, "bruh", nullptr, nullptr);
    if (!appdata.window) {
        glfwTerminate();
        throw std::runtime_error("failed to create window");
    }
    createVkInstance(&appdata.instance);

    while (!glfwWindowShouldClose(appdata.window)) {
        glfwPollEvents();
    }

    cleanup(appdata);
}
