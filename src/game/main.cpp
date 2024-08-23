#include <thread>
#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include <chrono>

#include "../engine/cleanupengine.h"
#include "../engine/enginedata.h"
#include "../engine/initengine.h"
#include "../engine/models.h"
#include "../engine/vulkan/buffers.h"
#include "../engine/vulkan/rendering.h"
#include "../utils/debugprint.h"
#include "gamedata.h"

int main() {

    bool windowShouldClose = false;
    auto startTime         = std::chrono::high_resolution_clock::now();
    auto endTime           = std::chrono::high_resolution_clock::now();
    auto targetTime        = std::chrono::milliseconds(10);
    auto renderTime        = endTime - startTime;

    Model model = {.vertices = {{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}},

                   .indices = {0, 1, 2, 2, 3, 0}};

    Data     data{};
    GameData gameData{};
    gameData.models.push_back(model);

    debugnote("created index and vertex buffers");
    initEngine(data);
    createModel(data.device,
                data.physicalDevice,
                data.queues.transferQueue,
                data.commandResources.commandPool,
                data.queueFamilyIndices,
                gameData.models[0],
                data.vertexBuffer,
                data.indexBuffer);
    createUniformBuffers(data.device, data.physicalDevice, data.queueFamilyIndices, data.uniformBuffers, data.MAX_FRAMES_IN_FLIGHT);

    while (!glfwWindowShouldClose(data.windowResources.windowPointer)) {
        startTime = std::chrono::high_resolution_clock::now();

        glfwPollEvents();
        drawFrame(data, gameData);

        endTime    = std::chrono::high_resolution_clock::now();
        renderTime = endTime - startTime;
        std::this_thread::sleep_for(targetTime - renderTime);
    }
    cleanup(data);

    debugnote("renderTime: " << std::chrono::duration_cast<std::chrono::microseconds>(renderTime).count());

    debugdata("end");
    return 0;
}
