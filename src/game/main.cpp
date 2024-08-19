#include <thread>
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

    auto startTime     = std::chrono::high_resolution_clock::now();
    auto endTime       = std::chrono::high_resolution_clock::now();
    auto frameTime     = endTime - startTime;
    auto sleepDuration = std::chrono::milliseconds(10) - frameTime;
    while (!glfwWindowShouldClose(data.window.windowPointer)) {
        startTime = std::chrono::high_resolution_clock::now();
        glfwPollEvents();
        drawFrame(data);
        endTime       = std::chrono::high_resolution_clock::now();
        frameTime     = endTime - startTime;
        sleepDuration = (std::chrono::milliseconds(10) - frameTime);
        std::this_thread::sleep_for(sleepDuration);
    }
    auto frameTimeMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(sleepDuration + frameTime).count();
    debugnote("frametime: " << frameTimeMilliseconds);

    cleanup(data);
}
