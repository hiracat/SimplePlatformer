#pragma once
#include <cstdint>

#include <GLFW/glfw3.h>

struct Window {
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 800;
    GLFWwindow* windowPointer;
};

void initializeWindow(Window& window);
