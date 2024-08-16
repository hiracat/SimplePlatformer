#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include <chrono>

#include "../engine/cleanupengine.h"
#include "../engine/enginedata.h"
#include "../engine/initengine.h"
#include "../engine/vulkan/rendering.h"
#include "../utils/debugprint.h"

int main() {

    Data data{};

    initEngine(data);

    bool windowShouldClose = false;

    auto startTime = std::chrono::high_resolution_clock::now();
    auto endTime   = std::chrono::high_resolution_clock::now();
    auto duration  = endTime - startTime;
    while (!glfwWindowShouldClose(data.window.windowPointer)) {
        startTime = std::chrono::high_resolution_clock::now();
        glfwPollEvents();
        drawFrame(data);
        endTime  = std::chrono::high_resolution_clock::now();
        duration = endTime - startTime;
    }
    auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    debugnote("frametime: " << frameTime);

    cleanup(data);
}
