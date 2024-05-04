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

void initializeWindow(GLFWwindow*& window, uint32_t width, uint32_t height, const char* name);
