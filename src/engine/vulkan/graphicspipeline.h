#pragma once
#include <vulkan/vulkan.h>

#include "swapchain.h"

void createGraphicsPipeline(const VkDevice&   device,
                            const Swapchain&  swapchain,
                            VkPipelineLayout& pipelineLayout,
                            VkRenderPass&     renderPass,
                            VkPipeline&       graphicsPipeline);
