#include "../vertex.h"
#include <vector>
#include "../enginedata.h"

void initVulkan(VulkanObjects& vulkanObjects, Window& window);
void createRenderingObjects(RenderingObjects& renderingObjects, RenderingResources& resources, const VkDevice device, const VkPhysicalDevice physicalDevice, const Window& window);
