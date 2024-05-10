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
    AppData appdata{}; // this struct holds all the stuff that has to be global
    initEngine(appdata);

    bool windowShouldClose = false;

    while (!glfwWindowShouldClose(appdata.window.windowPointer)) {
        glfwPollEvents();
        drawFrame(appdata.device,
                  appdata.syncObjects,
                  appdata.swapchain.swapchain,
                  appdata.commandBuffer,
                  appdata.swapchain.extent,
                  appdata.renderPass,
                  appdata.swapchainFramebuffers,
                  appdata.graphicsPipeline,
                  appdata.graphicsQueue,
                  appdata.presentQueue);
    }
    cleanup(appdata);
}
