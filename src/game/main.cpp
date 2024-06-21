#include <chrono>
#include <vulkan/vulkan_core.h>

#include <GLFW/glfw3.h>

#include "../engine/cleanupengine.h"
#include "../engine/enginedata.h"
#include "../engine/initengine.h"
#include "../engine/vulkan/rendering.h"

int main() {

    Data data{};

    initEngine(data);

    bool windowShouldClose = false;
    auto startTime         = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(data.window.windowPointer)) {
        glfwPollEvents();
        auto endTime  = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
        startTime = endTime;
        drawFrame(data);
    }
    cleanup(data);
}
