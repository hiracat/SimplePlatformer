#include <vulkan/vulkan.h>

#include <vector>

#include "../enginedata.h"
#include "../vertex.h"

void createVertexBuffer(const std::vector<Vertex>& vertices,
                        const VkDevice             device,
                        const VkPhysicalDevice     physicalDevice,
                        Buffer&                    buffer,
                        const VkCommandPool        commandPool,
                        const VkQueue              transferQueue,
                        const QueueFamilyIndices&  queueFamilyIndices);

void createIndexBuffer(const std::vector<uint32_t>& indices,
                       const VkDevice               device,
                       const VkPhysicalDevice       physicalDevice,
                       Buffer&                      buffer,
                       const VkCommandPool          commandPool,
                       const VkQueue                transferQueue,
                       const QueueFamilyIndices&    queueFamilyIndices);
