#include <cstddef>
#include <glm/fwd.hpp>
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
#include "../engine/vulkan/ubo.h"
#include "../utils/debugprint.h"
#include "gamedata.h"

int main() {

    bool windowShouldClose = false;
    auto frameStartTime    = std::chrono::high_resolution_clock::now();
    auto endTime           = std::chrono::high_resolution_clock::now();
    auto targetTime        = std::chrono::milliseconds(10);
    auto renderTime        = endTime - frameStartTime;
    auto startTime         = std::chrono::high_resolution_clock::now();

    Model player = {.vertices = {{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                 {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                 {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                 {{-0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}}},

                    .indices = {0, 1, 2, 2, 3, 0}};
    Model floor  = {.vertices = {{{-0.9f, 0.9f}, {1.0f, 1.0f, 1.0f}},
                                 {{0.9f, 0.9f}, {1.0f, 1.0f, 1.0f}},
                                 {{0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
                                 {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}},

                    .indices = {0, 1, 2, 2, 3, 0}};

    Data     data{};
    GameData gameData{};
    gameData.models.push_back(player);
    gameData.models.push_back(floor);

    debugnote("created index and vertex buffers");
    initEngine(data);
    for (size_t i = 0; i < gameData.models.size(); i++) {
        createModel(data.device,
                    data.physicalDevice,
                    data.queues.transfer,
                    data.commandResources.pool,
                    data.queueFamilyIndices,
                    gameData.models[i]);
    }
    createUniformBuffers(data.device,
                         data.physicalDevice,
                         data.queueFamilyIndices,
                         data.transformResources.uniformBuffers,
                         data.MAX_FRAMES_IN_FLIGHT,
                         gameData.models.size());
    createDescriptorPool(data.device, data.descriptorResources.pool, data.MAX_FRAMES_IN_FLIGHT);
    createDescriptorSets(data.device,
                         data.transformResources.uniformBuffers,
                         data.descriptorResources.pool,
                         data.descriptorResources.sets,
                         data.transformResources.descriptorSetLayout,
                         data.MAX_FRAMES_IN_FLIGHT);

    while (!glfwWindowShouldClose(data.windowResources.pointer)) {
        frameStartTime = std::chrono::high_resolution_clock::now();

        glfwPollEvents();
        int key = 0;
        for (int testkey = GLFW_KEY_SPACE; testkey <= GLFW_KEY_LAST; ++testkey) {
            int state = glfwGetKey(data.windowResources.pointer, testkey);
            if (state == GLFW_PRESS) {
                key = testkey;
                debugdata("key: " << key << "pressed");
                break;
            }
        }
        if (key == GLFW_KEY_RIGHT) {
            gameData.models[0].position.x += .01;
        } else if (key == GLFW_KEY_LEFT) {
            gameData.models[0].position.x -= .01;
        } else if (key == GLFW_KEY_UP) {
            gameData.models[0].position.y -= .01;
        } else if (key == GLFW_KEY_DOWN) {
            gameData.models[0].position.y += .01;
        } else if (key == GLFW_KEY_W) {
            gameData.models[0].position.z += .01;
        } else if (key == GLFW_KEY_S) {
            gameData.models[0].position.z -= .01;
        }
        drawFrame(data, gameData, startTime);

        endTime    = std::chrono::high_resolution_clock::now();
        renderTime = endTime - frameStartTime;
        std::this_thread::sleep_for(targetTime - renderTime);
    }
    cleanup(data, gameData);

    debugnote("renderTime: " << std::chrono::duration_cast<std::chrono::microseconds>(renderTime).count());

    debugdata("end");
    return 0;
}
