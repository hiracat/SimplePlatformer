#include <iostream>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../engine/window.h"
#include "../utils/debugprint.h"
#include "enginedata.h"
#include "vulkan/buffers.h"
#include "window.h"
#include "vulkan/vulkan.h"


void initEngine(Data& Data) {
    debugnote("ig we're at it again yay have fun");
    std::cout << "glfwinit says: " << glfwInit() << std::endl;

    initializeWindow(Data.window, &Data.framebufferResized);
    initVulkan(Data.vulkanObjects, Data.window);
    createRenderingObjects(Data.renderingObjects, Data.resources, Data.vulkanObjects.device, Data.vulkanObjects.physicalDevice, Data.window);

}
