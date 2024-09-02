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
                                 {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}},

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
    data.indexBuffers.resize(gameData.models.size());
    data.vertexBuffers.resize(gameData.models.size());
    for (size_t i = 0; i < gameData.models.size(); i++) {
        createModel(data.device,
                    data.physicalDevice,
                    data.queues.transfer,
                    data.commandResources.pool,
                    data.queueFamilyIndices,
                    gameData.models[i],
                    data.vertexBuffers[i],
                    data.indexBuffers[i]);
    }
    createUniformBuffers(
        data.device, data.physicalDevice, data.queueFamilyIndices, data.transformResources.uniformBuffers, data.MAX_FRAMES_IN_FLIGHT);
    createDescriptorPool(data.device, data.descriptorResources.pool, data.MAX_FRAMES_IN_FLIGHT);
    createDescriptorSets(data.device,
                         data.transformResources.uniformBuffers,
                         data.descriptorResources.pool,
                         data.descriptorResources.sets,
                         data.transformResources.descriptorSetLayout,
                         data.MAX_FRAMES_IN_FLIGHT);

    glm::vec3 offset = {};
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
            offset.x += .01;
        } else if (key == GLFW_KEY_LEFT) {
            offset.x -= .01;
        } else if (key == GLFW_KEY_UP) {
            offset.y -= .01;
        } else if (key == GLFW_KEY_DOWN) {
            offset.y += .01;
        } else if (key == GLFW_KEY_W) {
            offset.z += .01;
        } else if (key == GLFW_KEY_S) {
            offset.z -= .01;
        }
        updateUniformBuffers(data.currentFrame, startTime, data.swapchain.extent, data.transformResources.uniformBuffers, offset);
        drawFrame(data, gameData, startTime);

        endTime    = std::chrono::high_resolution_clock::now();
        renderTime = endTime - frameStartTime;
        std::this_thread::sleep_for(targetTime - renderTime);
    }
    cleanup(data);

    debugnote("renderTime: " << std::chrono::duration_cast<std::chrono::microseconds>(renderTime).count());

    debugdata("end");
    return 0;
}
