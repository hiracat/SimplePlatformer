#pragma once
#include "../game/gamedata.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <vector>

struct Swapchain {
    VkSwapchainKHR swapchain;
    VkFormat       format;
    VkExtent2D     extent;
    VkSwapchainKHR oldSwapChain{};
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentModes;
};

struct WindowResources {
    const uint32_t WIDTH  = 800;
    const uint32_t HEIGHT = 600;
    GLFWwindow*    windowPointer{};
    VkSurfaceKHR   surface{};
    const char*    name = "hello ur mother";
};

struct SyncResources {
    std::vector<VkSemaphore> imageAvailableSemaphores, renderFinishedSemaphores;
    std::vector<VkFence>     inFlightFences;
};

struct Queues {
    VkQueue graphicsQueue{};
    VkQueue presentQueue{};
    VkQueue transferQueue{};
};

struct InstanceResources {
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    VkDebugUtilsMessengerEXT       debugMessenger{};
    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};

struct CommandResources {
    VkCommandPool                commandPool{};
    std::vector<VkCommandBuffer> commandBuffers{};
};

struct PipelineResources {
    VkPipeline       graphicsPipeline{};
    VkPipelineLayout pipelineLayout{};
    VkRenderPass     renderPass{};
};

struct SwapchainResources {
    std::vector<VkImage>       images;
    std::vector<VkImageView>   imageViews;
    std::vector<VkFramebuffer> swapchainFramebuffers{};
};

struct Buffer {
    VkBuffer       buffer;
    VkDeviceMemory memory;
};

struct Data {
    VkInstance        instance{};
    VkDevice          device{};
    VkPhysicalDevice  physicalDevice{};
    InstanceResources instanceResources;

    WindowResources    windowResources{};
    Swapchain          swapchain{};
    SwapchainResources swapchainResources;
    SyncResources      syncResources{};

    CommandResources  commandResources;
    PipelineResources pipelineResources;
    Queues            queues;

    Buffer vertexBuffer{};
    Buffer indexBuffer{};

    uint32_t  currentFrame         = 0;
    bool      framebufferResized   = false;
    const int MAX_FRAMES_IN_FLIGHT = 2;
    GameData  gamedata{};
};
