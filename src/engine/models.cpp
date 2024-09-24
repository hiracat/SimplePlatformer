#include "../engine/vulkan/ubo.h"
#include "engine.h"
#include "vulkan/buffers.h"
#include <vector>
#include <vulkan/vulkan_core.h>

void createModels(EngineData* engineData, std::vector<Model>* models) {

    for (auto& model : *models) {
        createVertexBuffer(model.vertices,
                           engineData->vulkanData.device,
                           engineData->vulkanData.physicalDevice,
                           model.vertexBuffer,
                           engineData->renderData.commandResources.pool,
                           engineData->vulkanData.queues.transfer,
                           engineData->vulkanData.queueFamilyIndices);

        createIndexBuffer(model.indices,
                          engineData->vulkanData.device,
                          engineData->vulkanData.physicalDevice,
                          model.indexBuffer,
                          engineData->renderData.commandResources.pool,
                          engineData->vulkanData.queues.transfer,
                          engineData->vulkanData.queueFamilyIndices);
    }

    createUniformBuffers(engineData->vulkanData, models->size(), &engineData->renderData.transformResources.uniformBuffers);

    createDescriptorPool(engineData->vulkanData, &engineData->renderData.descriptorResources.pool);

    createDescriptorSets(engineData->renderData, &engineData->renderData.descriptorResources.sets);
}
