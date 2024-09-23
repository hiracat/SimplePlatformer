#pragma once
#include "vertex.h"
#include <GLFW/glfw3.h>
#include <glm/matrix.hpp>
#include <optional>
#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <vector>

struct Swapchain {
    VkSwapchainKHR swapchain;
    VkSwapchainKHR oldSwapChain{};
    VkFormat       format;
    VkExtent2D     extent;
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphics;
    std::optional<uint32_t> present;
    std::optional<uint32_t> transfer;

    bool isComplete() const;
    bool isSame() const;
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentModes;
};

struct SyncResources {
    std::vector<VkSemaphore> imageAvailable, renderFinished;
    std::vector<VkFence>     inFlight;
};

struct Queues {
    VkQueue graphics{};
    VkQueue present{};
    VkQueue transfer{};
};

struct CommandResources {
    std::vector<VkCommandBuffer> buffers{};
    VkCommandPool                pool{};
};

struct PipelineResources {
    VkPipeline       graphicsPipeline{};
    VkPipelineLayout pipelineLayout{};
    VkRenderPass     renderPass{};
};

struct SwapchainResources {
    std::vector<VkImage>       images;
    std::vector<VkImageView>   imageViews;
    std::vector<VkFramebuffer> framebuffers{};
};

struct Buffer {
    VkBuffer       buffer;
    VkDeviceMemory memory;
};

struct UniformBuffer {
    Buffer buffer;
    void*  mappedMemory;
};

// TODO: this is technical debt and must be replaced with dynamic allignment at some point
struct alignas(4) MVPMatricies {
    glm::mat4 model; // glm is compatable with shader format so memcpy works just fine to transfer
    glm::mat4 view;
    glm::mat4 projection;
};

struct Model {
    std::vector<Vertex>   vertices{};
    std::vector<uint32_t> indices{};
    glm::vec3             position{};

    Buffer vertexBuffer{};
    Buffer indexBuffer{};
};
struct TransformResources {
    MVPMatricies               matricies{};
    VkDescriptorSetLayout      descriptorSetLayout{};
    std::vector<UniformBuffer> uniformBuffers{};
};

struct DescriptorResources {
    VkDescriptorPool             pool{};
    std::vector<VkDescriptorSet> sets;
};

struct WindowData {
    GLFWwindow*    window{};
    VkSurfaceKHR   surface{};
    bool           framebufferResized = false;
    const uint32_t WIDTH              = 800;
    const uint32_t HEIGHT             = 600;
    const char*    name               = "hello ur mother";
};

struct VulkanData {
    VulkanData(VkSurfaceKHR& surface, const uint32_t& MAX_FRAMES_IN_FLIGHT, const WindowData& windowData)
        : surface(surface), MAX_FRAMES_IN_FLIGHT(MAX_FRAMES_IN_FLIGHT), windowData((windowData)) {};

    VkInstance                     instance{};
    VkDebugUtilsMessengerEXT       debugMessenger{};
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkDevice           device{};
    VkPhysicalDevice   physicalDevice{};
    Queues             queues;
    QueueFamilyIndices queueFamilyIndices;

    const VkSurfaceKHR& surface;
    const uint32_t&     MAX_FRAMES_IN_FLIGHT;
    const WindowData&   windowData;
};

struct RendererData {
    RendererData(VkDevice& device, const uint32_t& MAX_FRAMES_IN_FLIGHT, const WindowData& windowData)
        : device(device), MAX_FRAMES_IN_FLIGHT(MAX_FRAMES_IN_FLIGHT), windowData(windowData) {};
    Swapchain          swapchain{};
    SwapchainResources swapchainResources;
    SyncResources      syncResources{};

    CommandResources    commandResources;
    PipelineResources   pipelineResources;
    DescriptorResources descriptorResources;
    TransformResources  transformResources;

    const VkDevice&   device;
    const uint32_t&   MAX_FRAMES_IN_FLIGHT;
    const WindowData& windowData;
    /*const Queues&   queues;*/
};

struct EngineData {
    uint32_t       currentFrame         = 0;
    const uint32_t MAX_FRAMES_IN_FLIGHT = 2;

    WindowData   windowData{};
    VulkanData   vulkanData{windowData.surface, MAX_FRAMES_IN_FLIGHT, windowData};
    RendererData renderData{vulkanData.device, MAX_FRAMES_IN_FLIGHT, windowData};
};
