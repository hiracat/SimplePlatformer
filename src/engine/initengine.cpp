#include <iostream>
#include <vector>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../engine/window.h"
#include "../utils/debugprint.h"
#include "enginedata.h"
#include "vertex.h"
#include "vulkan/buffers.h"
#include "window.h"
#include "vulkan/vulkan.h"


void initEngine(EngineData& enginedata, const std::vector<Vertex>& renderData) {
    debugnote("ig we're at it again yay have fun");
    std::cout << "glfwinit says: " << glfwInit() << std::endl;

    initializeWindow(enginedata.window, &enginedata.framebufferResized);
    initVulkan(enginedata, renderData);

}
