#include <stdexcept>

#include <GLFW/glfw3.h>

#include "window.h"

void initializeWindow(Window& window) {
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwInit();
    window.windowPointer = glfwCreateWindow(window.WIDTH, window.HEIGHT, "bruh", nullptr, nullptr);

    if (!window.windowPointer) {
        glfwTerminate();
        throw std::runtime_error("failed to create window");
    }
}
