#include <chrono>
#include <cstdint>
#include <iostream>
#include <vulkan/vulkan_core.h>

#include <GLFW/glfw3.h>
// #include <vulkan/vk_enum_string_helper.h>

#include "../engine/cleanupengine.h"
#include "../engine/enginedata.h"
#include "../engine/initengine.h"
#include "../engine/vertex.h"
#include "../engine/vulkan/rendering.h"

int main() {

    Data data{}; // this struct holds all the stuff that has to be global

    initEngine(data);

    bool windowShouldClose = false;
    auto startTime         = std::chrono::high_resolution_clock::now();

   // while (!glfwWindowShouldClose(data.window.windowPointer)) {
   while (true){
        glfwPollEvents();
        // std::cout << "window should close? " << glfwWindowShouldClose(data.window.windowPointer) << std::endl;
        auto endTime  = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
        startTime = endTime;
        drawFrame(data);
    }
    cleanup(data);
}
