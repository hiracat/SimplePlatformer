#pragma once
#include <cstdint>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

struct Window {
    const uint32_t WIDTH  = 800;
    const uint32_t HEIGHT = 600;
    GLFWwindow*    windowPointer;
    VkSurfaceKHR   surface;
};

void initializeWindow(GLFWwindow*& window, const uint32_t width, const uint32_t height, const char* name, bool* windowResized);
void createSurface(const VkInstance instance, GLFWwindow* window, VkSurfaceKHR* surface);
