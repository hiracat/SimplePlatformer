#pragma once
#include <cstdint>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

struct Window {
    const uint32_t WIDTH  = 800;
    const uint32_t HEIGHT = 600;
    GLFWwindow*    windowPointer{};
    VkSurfaceKHR   surface{};
    const char* name = "slidy thing";
};

void initializeWindow(Window& window, bool* frameBufferResized);
void createSurface(const VkInstance instance, Window& window);
