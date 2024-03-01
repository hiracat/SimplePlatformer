#include <cstdint>
#include <stdexcept>

#include <GLFW/glfw3.h>

#include "window.h"

void initializeWindow(GLFWwindow*& window, uint32_t width, uint32_t height, const char* name) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(width, height, name, nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        throw std::runtime_error("failed to create window");
    }
}
