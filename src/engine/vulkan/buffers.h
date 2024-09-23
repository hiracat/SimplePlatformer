#include <cstdint>
#include <vulkan/vulkan.h>

#include <vector>

#include "../engine.h"
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

void createUniformBuffers(const VulkanData& data, const uint32_t numModels, std::vector<UniformBuffer>* buffers);

void updateUniformBuffers(uint32_t                                       currentImage,
                          std::chrono::high_resolution_clock::time_point startTime,
                          const VkExtent2D&                              swapchainExtent,
                          std::vector<UniformBuffer>                     buffers,
                          glm::vec3                                      offset,
                          uint32_t                                       dynamicOffset);
