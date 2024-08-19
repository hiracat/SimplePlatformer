#include <cstdlib>
#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include <vulkan/vulkan_core.h>

#include "../utils/debugprint.h"
#include "enginedata.h"
void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto frameBufferResized = reinterpret_cast<bool*>(glfwGetWindowUserPointer(window));
    *frameBufferResized     = true;
}

void initializeWindow(VkInstance instance, WindowResources& windowData, bool* frameBufferResized) {
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

    int reply2;
    if ((reply2 = glfwCreateWindowSurface(instance, windowData.windowPointer, nullptr, &windowData.surface)) != VK_SUCCESS) {
        debugerror("failed to create window surface: " << reply2);
    }
}
