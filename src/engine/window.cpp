#include <cstdint>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vk_enum_string_helper.h>

#include "../utils/debugprint.h"
#include "window.h"

void initializeWindow(GLFWwindow*& window, const uint32_t width, const uint32_t height, const char* name) {
    // disables the automatic opengl context creation
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // resizing has to be done specially so we disable it here
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(width, height, name, nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        throw std::runtime_error("failed to create window");
    }
}

void createSurface(const VkInstance instance, GLFWwindow* window, VkSurfaceKHR* surface) {

    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, surface);

    if (result != VK_SUCCESS) {
        debugerror("creating window failed with the error: " << string_VkResult(result));
        throw std::runtime_error("failed to create window surface");
    }
};
