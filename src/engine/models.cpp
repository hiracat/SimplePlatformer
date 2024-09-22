#include "engine.h"
#include "vulkan/buffers.h"
#include <vulkan/vulkan_core.h>

void createModel(const EngineData& engineData, Model* model) {
    createVertexBuffer(model->vertices,
                       engineData.vulkanData.device,
                       engineData.vulkanData.physicalDevice,
                       model->vertexBuffer,
                       engineData.renderData.commandResources.pool,
                       engineData.vulkanData.queues.transfer,
                       engineData.vulkanData.queueFamilyIndices);

    createIndexBuffer(model->indices,
                      engineData.vulkanData.device,
                      engineData.vulkanData.physicalDevice,
                      model->indexBuffer,
                      engineData.renderData.commandResources.pool,
                      engineData.vulkanData.queues.transfer,
                      engineData.vulkanData.queueFamilyIndices);
}
