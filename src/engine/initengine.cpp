#include "../engine/window.h"
#include "../utils/debugprint.h"
#include "enginedata.h"
#include "vulkan/buffers.h"
#include "vulkan/vulkan.h"
#include "window.h"

void initEngine(Data& data) {
    debugnote("ig we're at it again yay have fun");

    initializeWindow(data.window, &data.framebufferResized);
    initVulkan(data.vulkanObjects, data.window);
    createRenderingObjects(data.renderingObjects,
                           data.resources,
                           data.vulkanObjects.device,
                           data.vulkanObjects.physicalDevice,
                           data.window,
                           data.window.surface);
}
