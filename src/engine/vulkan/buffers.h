#include "../vertex.h"
#include <vector>
#include <vulkan/vulkan_core.h>

void createVertexBuffer(const std::vector<Vertex>& vertices,
                        const VkDevice             device,
                        const VkPhysicalDevice     physicalDevice,
                        VkBuffer&                  vertexBuffer,
                        VkDeviceMemory&            vertexBufferMemory);
