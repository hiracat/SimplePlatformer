#include <cstdlib>
#include <stdexcept>
#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include <vulkan/vulkan_core.h>

#include "../utils/debugprint.h"
#include "window.h"

void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto frameBufferResized = reinterpret_cast<bool*>(glfwGetWindowUserPointer(window));
    *frameBufferResized     = true;
}

void initializeWindow(Window& windowData, bool* frameBufferResized) {
    int reply = glfwInit();
    debugnote("reply: " << reply);
    // disables the automatic opengl context creation
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    windowData.windowPointer = glfwCreateWindow(windowData.WIDTH, windowData.HEIGHT, windowData.name, nullptr, nullptr);
    if (windowData.windowPointer == nullptr) {
        debugerror("window pointer is nullptr");
    }

    glfwSetWindowUserPointer(windowData.windowPointer, frameBufferResized);
    glfwSetFramebufferSizeCallback(windowData.windowPointer, framebufferResizeCallback);
}

void createSurface(const VkInstance instance, GLFWwindow* window, VkSurfaceKHR* surface) {
    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, surface);

    if (result != VK_SUCCESS) {
        debugerror("failed to create surface");
    }
};
