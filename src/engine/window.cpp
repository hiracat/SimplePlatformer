#include <cstdlib>
#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include <vulkan/vulkan_core.h>

#include "../utils/debugprint.h"
#include "engine.h"
void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto frameBufferResized = reinterpret_cast<bool*>(glfwGetWindowUserPointer(window));
    *frameBufferResized     = true;
}

void initWindow(const VulkanData& vulkanData, WindowData* windowData) {
    int reply = glfwInit();
    debugnote("reply: " << reply);
    // disables the automatic opengl context creation

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    windowData->window = glfwCreateWindow(windowData->WIDTH, windowData->HEIGHT, windowData->name, nullptr, nullptr);
    if (windowData->window == nullptr) {
        debugerror("window pointer is nullptr");
    }

    glfwSetWindowUserPointer(windowData->window, &windowData->framebufferResized);
    glfwSetFramebufferSizeCallback(windowData->window, framebufferResizeCallback);

    int reply2;
    if ((reply2 = glfwCreateWindowSurface(vulkanData.instance, windowData->window, nullptr, &windowData->surface)) != VK_SUCCESS) {
        debugerror("failed to create window surface: " << reply2);
    }
}
