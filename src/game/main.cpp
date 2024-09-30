#include <cmath>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <thread>
#include <vector>
#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include <chrono>
#include <vulkan/vulkan_core.h>

#include "../engine/engine.h"
#include "../engine/models.h"
#include "../engine/vulkan/buffers.h"
#include "../engine/vulkan/ubo.h"
#include "../utils/debugprint.h"

void cleanupModel(const VkDevice& device, Model* model) {
    vkDeviceWaitIdle(device);
    vkDestroyBuffer(device, model->vertexBuffer.buffer, nullptr);
    vkFreeMemory(device, model->vertexBuffer.memory, nullptr);

    vkDestroyBuffer(device, model->indexBuffer.buffer, nullptr);
    vkFreeMemory(device, model->indexBuffer.memory, nullptr);
}

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

std::vector<int> getMouseButton(GLFWwindow* window) {
    std::vector<int> buttons{};
    for (int testButton = GLFW_MOUSE_BUTTON_LEFT; testButton <= GLFW_MOUSE_BUTTON_LAST; ++testButton) {
        int state = glfwGetMouseButton(window, testButton);
        if (state == GLFW_PRESS) {
            buttons.push_back(testButton);
            debugdata("mouse: " << testButton << "pressed");
        }
    }
    return buttons;
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

    Model floor = {.vertices = {{{-15.0f, 15.0f}, {0.2f, 0.8f, 0.2f}},
                                {{15.0f, 15.0f}, {0.2f, 0.6f, 0.1f}},
                                {{15.0f, -15.0f}, {0.2f, 0.0f, 0.5f}},
                                {{-15.0f, -15.0f}, {0.1f, 0.0f, 0.8f}}},
                   .indices  = {0, 1, 2, 2, 3, 0}};

    Model bullet = {.vertices = {{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
                                 {{0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}},
                                 {{0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}},
                                 {{-0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}}},
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
    int    modelLoaded = 5;

    std::vector<int> keys    = getPressedKeys(engineData.windowData.window);
    std::vector<int> buttons = getMouseButton(engineData.windowData.window);

    double mouseX, mouseY;
    int    windowWidth, windowHeight;

    std::vector<glm::vec3> bulletTarget;
    while (!glfwWindowShouldClose(engineData.windowData.window)) {
        glfwPollEvents();
        frameStartTime = std::chrono::high_resolution_clock::now();

        keys    = getPressedKeys(engineData.windowData.window);
        buttons = getMouseButton(engineData.windowData.window);
        glfwGetCursorPos(engineData.windowData.window, &mouseX, &mouseY);
        glfwGetWindowSize(engineData.windowData.window, &windowWidth, &windowHeight);
        float     ndcX = (2.0f * mouseX) / windowWidth - 1.0f;
        float     ndcY = 1.0f - (2.0f * mouseY) / windowHeight; // in screen space y increases downwards
        glm::vec4 ndcCoords(ndcX, ndcY, 0.0f, 1.0f);

        glm::mat4 viewMatrix       = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), windowWidth / (float)windowHeight, 0.1f, 10.0f);

        glm::mat4 invProjectionMatrix = glm::inverse(projectionMatrix);
        glm::mat4 invViewMatrix       = glm::inverse(viewMatrix);

        glm::vec4 viewSpaceCoords = invProjectionMatrix * ndcCoords;
        viewSpaceCoords /= viewSpaceCoords.w;
        glm::vec4 worldSpaceCoords = invViewMatrix * viewSpaceCoords;

        glm::vec3 worldPos = glm::vec3(worldSpaceCoords);

        glm::vec3 rayOrigin    = {0, 0, 10}; // Assuming you have the camera position
        glm::vec3 rayDirection = glm::normalize(worldPos - rayOrigin);

        glm::vec3 intersectionPoint;
        if (rayDirection.z != 0.0f) {
            float t           = -rayOrigin.z / rayDirection.z; // Solve for t when z = 0
            intersectionPoint = rayOrigin + t * rayDirection;

            debugdata("Intersection Point at z=0: (" << intersectionPoint.x << ", " << intersectionPoint.y << ", " << intersectionPoint.z
                                                     << ")");
        }

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

        for (auto button : buttons) {
            if (button == GLFW_MOUSE_BUTTON_LEFT && modelLoaded == 0) {

                models.push_back(bullet);

                addModel(&engineData, &models.back());
                models.back().position = models[1].position;
                modelLoaded            = 5;
                bulletTarget.push_back(glm::normalize(intersectionPoint - models.back().position));
            }
            if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                for (int i = 0; i < models.size() - 2; i++) {
                    cleanupModel(engineData.vulkanData.device, &models.back());
                    models.pop_back();
                    bulletTarget.clear();
                }
            }
        }
        modelLoaded--;

        if (buttons.size() == 0) {
            modelLoaded = 5;
        }

        float speed = .2;
        for (int i = 2; i < models.size(); i++) {
            models[i].position += bulletTarget[i - 2] * speed;
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
