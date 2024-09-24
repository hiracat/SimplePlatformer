#pragma once
#include <vulkan/vulkan.h>

#include "../engine.h"

void createGraphicsPipeline(const RendererData& renderData, PipelineResources* resources, VkDescriptorSetLayout* descriptorSetLayout);
