#include <cstring>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vk_enum_string_helper.h>

#include "../engine/cleanupengine.h"
#include "../engine/initengine.h"
#include "../engine/vulkan/rendering.h"
#include "../engine/window.h"
#include "main.h"

int main() {
    EngineData enginedata{}; // this struct holds all the stuff that has to be global
    initEngine(enginedata);

    bool windowShouldClose = false;

    while (!glfwWindowShouldClose(enginedata.window.windowPointer)) {
        glfwPollEvents();
        drawFrame(enginedata.device,
                  enginedata.syncObjects,
                  enginedata.swapchain.swapchain,
                  enginedata.commandBuffer,
                  enginedata.swapchain.extent,
                  enginedata.renderPass,
                  enginedata.swapchainFramebuffers,
                  enginedata.graphicsPipeline,
                  enginedata.graphicsQueue,
                  enginedata.presentQueue);
    }
    cleanup(enginedata);
}
