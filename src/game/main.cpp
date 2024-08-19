#include <thread>
#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include <chrono>

#include "../engine/cleanupengine.h"
#include "../engine/enginedata.h"
#include "../engine/initengine.h"
#include "../engine/vulkan/buffers.h"
#include "../engine/vulkan/rendering.h"
#include "../utils/debugprint.h"
#include "gamedata.h"

int main() {

    glfwInit();

    bool windowShouldClose = false;
    auto startTime         = std::chrono::high_resolution_clock::now();
    auto endTime           = std::chrono::high_resolution_clock::now();
    auto duration          = endTime - startTime;

    Model model = {.vertices = {{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}},

                   .indices = {0, 1, 2, 2, 3, 0}};

    Data data{};
    data.gamedata.models.push_back(model);

    debugnote("creqated index and vertex buffers");
    initEngine(data);
    createIndexBuffer(data.gamedata.models[0].indices,
                      data.device,
                      data.physicalDevice,
                      data.indexBuffer.buffer,
                      data.indexBuffer.memory,
                      data.windowResources.surface,
                      data.commandResources.commandPool,
                      data.queues.transferQueue);

    createVertexBuffer(data.gamedata.models[0].vertices,
                       data.device,
                       data.physicalDevice,
                       data.vertexBuffer.buffer,
                       data.vertexBuffer.memory,
                       data.windowResources.surface,
                       data.commandResources.commandPool,
                       data.queues.transferQueue);

    while (!glfwWindowShouldClose(data.windowResources.windowPointer)) {
        startTime = std::chrono::high_resolution_clock::now();

        glfwPollEvents();
        drawFrame(data, data.gamedata);

        endTime  = std::chrono::high_resolution_clock::now();
        duration = endTime - startTime;
    }
    cleanup(data);

    auto frameTime = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    debugnote("frametime: " << frameTime);
    return 0;
}
