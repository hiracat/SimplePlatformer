#include <cstring>
#include <vulkan/vulkan_core.h>

#include <GLFW/glfw3.h>
#include <vulkan/vk_enum_string_helper.h>

#include "../engine/cleanupengine.h"
#include "../engine/enginedata.h"
#include "../engine/initengine.h"
#include "../engine/vulkan/rendering.h"

int main() {
    EngineData enginedata{}; // this struct holds all the stuff that has to be global
    initEngine(enginedata);

    bool windowShouldClose = false;

    while (!glfwWindowShouldClose(enginedata.window.windowPointer)) {
        glfwPollEvents();
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
                  enginedata.framebufferResized

        );
    }
    cleanup(enginedata);
}
