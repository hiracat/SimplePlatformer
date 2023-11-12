#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "bruh", nullptr, nullptr);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}
