#include <cstdint>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// #include <vulkan/vk_enum_string_helper.h>

#include "window.h"

void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto windowResized = reinterpret_cast<bool*>(glfwGetWindowUserPointer(window));
    *windowResized     = true;
}

void initializeWindow(GLFWwindow*& window, const uint32_t width, const uint32_t height, const char* name, bool* windowResized) {
    // disables the automatic opengl context creation
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    glfwSetWindowUserPointer(window, windowResized);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    if (!window) {
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
