#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// #include <vulkan/vk_enum_string_helper.h>

#include "window.h"

void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto frameBufferResized = reinterpret_cast<bool*>(glfwGetWindowUserPointer(window));
    *frameBufferResized     = true;
}

void initializeWindow(Window& windowData, bool* frameBufferResized) {
    // disables the automatic opengl context creation
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    windowData.windowPointer = glfwCreateWindow(windowData.WIDTH, windowData.HEIGHT, windowData.name, nullptr, nullptr);
    glfwSetWindowUserPointer(windowData.windowPointer, frameBufferResized);
    glfwSetFramebufferSizeCallback(windowData.windowPointer, framebufferResizeCallback);

    if (!windowData.windowPointer) {
        glfwTerminate();
        throw std::runtime_error("failed to create window");
    }
}

void createSurface(const VkInstance instance, GLFWwindow* window, VkSurfaceKHR* surface) {
    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, surface);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface");
    }
};
