#include <vulkan/vulkan.h>

#include <vector>

#include "../vertex.h"

void createVertexBuffer(const std::vector<Vertex>& vertices,
                        const VkDevice             device,
                        const VkPhysicalDevice     physicalDevice,
                        VkBuffer&                  vertexBuffer,
                        VkDeviceMemory&            vertexBufferMemory);
