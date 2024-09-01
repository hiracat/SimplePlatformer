#pragma once
#include <vulkan/vulkan.h>

#include "../enginedata.h"

void createGraphicsPipeline(const VkDevice&        device,
                            const Swapchain&       swapchain,
                            PipelineResources&     resources,
                            VkDescriptorSetLayout& descriptorSetLayout);
