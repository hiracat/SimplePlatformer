#include <vector>

#include "../enginedata.h"
#include "../vertex.h"

void initVulkan(VulkanObjects& vulkanObjects, Window& window);
void createRenderingObjects(RenderingObjects&      renderingObjects,
                            RenderingResources&    resources,
                            const VkDevice         device,
                            const VkPhysicalDevice physicalDevice,
                            const Window&          window);
