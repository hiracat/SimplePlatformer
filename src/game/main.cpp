#include <cmath>
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

std::vector<int> getPressedKeys(GLFWwindow* window) {
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

    Model player = {.vertices = {{{-0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
                                 {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                 {{0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
                                 {{-0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}}},
                    .indices  = {0, 1, 2, 2, 3, 0}};

    Model floor = {.vertices = {{{-15.0f, 15.0f}, {0.2f, 0.2f, 0.2f}},
                                {{15.0f, 15.0f}, {0.2f, 0.2f, 0.2f}},
                                {{15.0f, -15.0f}, {0.2f, 0.2f, 0.2f}},
                                {{-15.0f, -15.0f}, {0.2f, 0.2f, 0.2f}}},
                   .indices  = {0, 1, 2, 2, 3, 0}};

    std::vector<Model> models{};
    models.push_back(floor);
    models.push_back(player);

    debugnote("created index and vertex buffers");
    EngineData engineData{};
    initEngine(&engineData);
    createModels(&engineData, &models);

    double yvelocity    = 0;
    bool   keyuppressed = false;
    int    key;

    while (!glfwWindowShouldClose(engineData.windowData.window)) {
        glfwPollEvents();
        frameStartTime        = std::chrono::high_resolution_clock::now();
        std::vector<int> keys = getPressedKeys(engineData.windowData.window);

        float     maxSpeed = .04;
        glm::vec2 velocity{};
        for (auto key : keys) {
            if (key == GLFW_KEY_RIGHT) {
                velocity.x += .04;
            }
            if (key == GLFW_KEY_LEFT) {
                velocity.x -= .04;
            }
            if (key == GLFW_KEY_UP) {
                velocity.y += .04;
            }
            if (key == GLFW_KEY_DOWN) {
                velocity.y -= .04;
            }
        }

        double magnatude = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        if (magnatude > 0) {
            velocity.x /= magnatude;
            velocity.y /= magnatude;

            velocity.x *= maxSpeed;
            velocity.y *= maxSpeed;
        }

        models[1].position.x += velocity.x;
        models[1].position.y += velocity.y;

        drawFrame(&engineData, models);

        endTime    = std::chrono::high_resolution_clock::now();
        renderTime = endTime - frameStartTime;
        std::this_thread::sleep_for(targetTime - renderTime);
    }
    cleanupEngine(&engineData, &models);

    std::cerr << "renderTime: " << std::chrono::duration_cast<std::chrono::microseconds>(renderTime).count() << std::endl;

    debugdata("shutting down");
    return 0;
}
