#include "../engine/window.h"
#include "../utils/debugprint.h"
#include "enginedata.h"
#include "vulkan/buffers.h"
#include "vulkan/vulkan.h"
#include "window.h"

void initEngine(Data& Data) {
    debugnote("ig we're at it again yay have fun");

    initializeWindow(Data.window, &Data.framebufferResized);
    initVulkan(Data.vulkanObjects, Data.window);
    createRenderingObjects(
        Data.renderingObjects, Data.resources, Data.vulkanObjects.device, Data.vulkanObjects.physicalDevice, Data.window);
}
