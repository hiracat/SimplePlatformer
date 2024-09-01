#pragma once
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

struct WindowResources {
    GLFWwindow*    pointer{};
    VkSurfaceKHR   surface{};
    const uint32_t WIDTH  = 800;
    const uint32_t HEIGHT = 600;
    const char*    name   = "hello ur mother";
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

struct InstanceResources {
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    VkDebugUtilsMessengerEXT       debugMessenger{};
    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
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

struct MVPMatricies {
    glm::mat4 model; // glm is compatable with shader format so memcpy works just fine to transfer
    glm::mat4 view;
    glm::mat4 projection;
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

struct Data {
    VkInstance        instance{};
    InstanceResources instanceResources;
    VkDevice          device{};
    VkPhysicalDevice  physicalDevice{};

    WindowResources windowResources{};

    Swapchain          swapchain{};
    SwapchainResources swapchainResources;
    SyncResources      syncResources{};

    CommandResources    commandResources;
    PipelineResources   pipelineResources;
    DescriptorResources descriptorResources;

    Queues             queues;
    QueueFamilyIndices queueFamilyIndices;

    Buffer vertexBuffer{};
    Buffer indexBuffer{};

    Buffer vertexBufferf{};
    Buffer indexBufferf{};

    TransformResources transformResources;

    uint32_t  currentFrame         = 0;
    bool      framebufferResized   = false;
    const int MAX_FRAMES_IN_FLIGHT = 2;
};
