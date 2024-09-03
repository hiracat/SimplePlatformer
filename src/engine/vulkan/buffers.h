#include <cstdint>
#include <vulkan/vulkan.h>

#include <vector>

#include "../enginedata.h"
#include "../vertex.h"
#include <chrono>

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

void createUniformBuffers(VkDevice                    device,
                          VkPhysicalDevice            physicalDevice,
                          QueueFamilyIndices          indices,
                          std::vector<UniformBuffer>& buffers,
                          const uint32_t              MAX_FRAMES_IN_FLIGHT,
                          uint32_t                    numModels);

void updateUniformBuffers(uint32_t                                       currentImage,
                          std::chrono::high_resolution_clock::time_point startTime,
                          const VkExtent2D&                              swapchainExtent,
                          std::vector<UniformBuffer>                     buffers,
                          glm::vec3                                      offset,
                          uint32_t                                       dynamicOffset);
