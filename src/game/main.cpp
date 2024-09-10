#include <cstddef>
#include <glm/fwd.hpp>
#include <thread>
#include <vector>
#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include <chrono>

#include "../engine/engine.h"
#include "../engine/models.h"
#include "../engine/vulkan/buffers.h"
#include "../engine/vulkan/ubo.h"
#include "../utils/debugprint.h"

std::vector<int> getPressedKey(GLFWwindow* window) {
    std::vector<int> keys{};
    for (int testkey = GLFW_KEY_SPACE; testkey <= GLFW_KEY_LAST; ++testkey) {
        int state = glfwGetKey(window, testkey);
        if (state == GLFW_PRESS) {
            keys.push_back(testkey);
            debugdata("key: " << testkey << "pressed");
        }
    }
    return keys;
}

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
                    .indices  = {0, 1, 2, 2, 3, 0}};

    Model floor = {.vertices = {{{-1.0f, -1.5f}, {1.0f, 1.0f, 1.0f}},
                                {{1.0f, -1.5f}, {1.0f, 1.0f, 1.0f}},
                                {{1.9f, -0.5f}, {1.0f, 1.0f, 0.0f}},
                                {{-1.9f, -0.5f}, {1.0f, 1.0f, 0.0f}}},
                   .indices  = {0, 1, 2, 2, 3, 0}};

    EngineData         engineData{};
    std::vector<Model> models{};
    models.push_back(player);
    models.push_back(floor);

    debugnote("created index and vertex buffers");
    initEngine(engineData);
    for (size_t i = 0; i < models.size(); i++) {
        /*createModel(data.device,*/
        /*            data.physicalDevice,*/
        /*            data.queues.transfer,*/
        /*            data.commandResources.pool,*/
        /*            data.queueFamilyIndices,*/
        /*            gameData.models[i]);*/
        createModel(engineData, models[i]);
    }
    /*createUniformBuffers(data.device,*/
    /*                     data.physicalDevice,*/
    /*                     data.queueFamilyIndices,*/
    /*                     data.transformResources.uniformBuffers,*/
    /*                     data.MAX_FRAMES_IN_FLIGHT,*/
    /*                     gameData.models.size());*/
    createUniformBuffers(engineData, models.size());

    /*createDescriptorPool(data.device, data.descriptorResources.pool, data.MAX_FRAMES_IN_FLIGHT);*/
    createDescriptorPool(engineData);

    createDescriptorSets(data.device,
                         data.transformResources.uniformBuffers,
                         data.descriptorResources.pool,
                         data.descriptorResources.sets,
                         data.transformResources.descriptorSetLayout,
                         data.MAX_FRAMES_IN_FLIGHT);
    createDescriptorSets(engineData);

    double yvelocity    = 0;
    bool   keyuppressed = false;
    int    key;

    while (!glfwWindowShouldClose(data.windowResources.pointer)) {
        glfwPollEvents();
        frameStartTime        = std::chrono::high_resolution_clock::now();
        std::vector<int> keys = getPressedKey(data.windowResources.pointer);

        for (auto key : keys) {
            if (key == GLFW_KEY_RIGHT) {
                models[0].position.x += .02;
            } else if (key == GLFW_KEY_LEFT) {
                models[0].position.x -= .02;
            } else if (key == GLFW_KEY_UP) {
                keyuppressed = true;
            } else if (key == GLFW_KEY_W) {
                models[0].position.z += .02;
            } else if (key == GLFW_KEY_S) {
                models[0].position.z -= .02;
            }
        }

        if (keyuppressed && (models[0].position.y < .0001f)) {
            yvelocity += .05;
            keyuppressed = false;
        }
        if (models[0].position.y > 0) {
            yvelocity -= .002; // gravity
        }
        models[0].position.y += yvelocity;

        debugdata("model position: " << models[0].position.y);
        debugdata("yvelocity: " << yvelocity);

        if (models[0].position.y < 0) {
            models[0].position.y = 0;
        }

        debugdata("model position: " << gameData.models[0].position.y);
        debugdata("yvelocity: " << yvelocity);
        debugdata("============================" << yvelocity);
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
