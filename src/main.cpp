#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

int main() {
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "bruh", nullptr, nullptr);

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

    {
        uint32_t supportedExtensionCount;
        std::vector<VkExtensionProperties> supportedExtensionProperties;

        vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, nullptr);
        supportedExtensionProperties.resize(supportedExtensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, supportedExtensionProperties.data());
        for (const VkExtensionProperties& extension : supportedExtensionProperties) {
            std::cout << "Available Extension: " << extension.extensionName << std::endl;
        }

        for (uint32_t i = 0; i < createInfo.enabledExtensionCount; i++) {
            const auto& extensionName = createInfo.ppEnabledExtensionNames[i];
            bool supported = false;

            for (const VkExtensionProperties& extension : supportedExtensionProperties) {
                if (strcmp(extensionName, extension.extensionName) == 0) {
                    supported = true;
                    break;
                }
            }
            if (!supported) {
                std::string message("an extension was requested but not available called: ");
                message.append(extensionName);
                throw std::runtime_error(message);
            }
        }
    }
    VkInstance instance{};
    VkResult result;

    if ((result = vkCreateInstance(&createInfo, nullptr, &instance)) != VK_SUCCESS) {
        std::cout << "result is: " << result << std::endl;
        throw std::runtime_error("failed to create instance!");
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);
    glfwTerminate();
}
