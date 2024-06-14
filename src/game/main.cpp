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
    const std::vector<Vertex> vertices = {{
                                              {0.0f, -0.5f},
                                              {1.0f, 1.0f, 1.0f},
                                          },
                                          {
                                              {0.5f, 0.5f},
                                              {0.0f, 1.0f, 0.0f},
                                          },
                                          {
                                              {-0.5f, 0.5f},
                                              {0.0f, 0.0f, 1.0f},
                                         }};

    EngineData enginedata{}; // this struct holds all the stuff that has to be global
    initEngine(enginedata, vertices);

    bool windowShouldClose = false;
    auto startTime         = std::chrono::high_resolution_clock::now();

   // while (!glfwWindowShouldClose(enginedata.window.windowPointer)) {
   while (true){
        glfwPollEvents();
        // std::cout << "window should close? " << glfwWindowShouldClose(enginedata.window.windowPointer) << std::endl;
        auto endTime  = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
        startTime = endTime;
        drawFrame(enginedata.device,
                  enginedata.syncObjects,
                  enginedata.swapchain,
                  enginedata.commandBuffers,
                  enginedata.swapchain.extent,
                  enginedata.renderPass,
                  enginedata.swapchainFramebuffers,
                  enginedata.graphicsPipeline,
                  enginedata.graphicsQueue,
                  enginedata.presentQueue,
                  enginedata.currentFrame,
                  enginedata.MAX_FRAMES_IN_FLIGHT,
                  enginedata.window,
                  enginedata.physicalDevice,
                  enginedata.window.surface,
                  enginedata.framebufferResized,
                  enginedata.vertexBuffer,
                  static_cast<uint32_t>(vertices.size())

        );
    }
    cleanup(enginedata);
}
