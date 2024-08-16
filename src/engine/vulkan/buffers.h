#include <vulkan/vulkan.h>

#include <vector>

#include "../vertex.h"

void createVertexBuffer(const std::vector<Vertex>& vertices,
                        const VkDevice             device,
                        const VkPhysicalDevice     physicalDevice,
                        VkBuffer&                  vertexBuffer,
                        VkDeviceMemory&            vertexBufferMemory,
                        const VkSurfaceKHR         surface,
                        const VkCommandPool        commandPool,
                        const VkQueue              transferQueue);
void createIndexBuffer(const std::vector<uint32_t>& indices,
                       const VkDevice               device,
                       const VkPhysicalDevice       physicalDevice,
                       VkBuffer&                    indexBuffer,
                       VkDeviceMemory&              indexBufferMemory,
                       const VkSurfaceKHR           surface,
                       const VkCommandPool          commandPool,
                       const VkQueue                transferQueue);
