#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vk_enum_string_helper.h>

#include "debugprint.h"
#include "physicaldevice.h"
#include "window.h"

#ifndef NDEBUG
#define ENABLE_VALIDATION_LAYERS
#endif

std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();

    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
                                             VkDebugUtilsMessageTypeFlagsEXT             messageType,
                                             const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                             void*                                       pUserData) {

    debugunknown("Validation layer says: " << pCallbackData->pMessage);
    return VK_FALSE;
}

bool checkExtensionSupported(const std::vector<VkExtensionProperties>& supportedExtensionProperties, const char* extensionName) {
    bool supported = false;
    for (const VkExtensionProperties& extension : supportedExtensionProperties) {
        if (strcmp(extensionName, extension.extensionName) == 0) {
            supported = true;
            break;
        }
    }
    return supported;
}

bool checkValidationLayerSupported(const std::vector<const char*>& validationLayers) {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;
        for (const VkLayerProperties& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            debugerror("requested layer " << layerName << " not available");
            return false;
        }
    }

    return true;
}

std::vector<const char*> getRequredExtensions() {
    uint32_t     glfwExtensionCount;
    const char** glfwExtensionNames = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    uint32_t                 requiredExtensionCount;
    std::vector<const char*> requiredExtensions(glfwExtensionNames, glfwExtensionNames + glfwExtensionCount);

#ifdef ENABLE_VALIDATION_LAYERS
    requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    return requiredExtensions;
}

void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                  .pNext = nullptr,
                  .flags = 0,

                  .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,

                  .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,

                  .pfnUserCallback = debugCallback,

                  .pUserData = nullptr};
}

void createVkInstance(VkInstance* instance, std::vector<const char*> validationLayers) {

    VkApplicationInfo appInfo{
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext              = nullptr,
        .pApplicationName   = "simple platformer",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName        = "no engine",
        .engineVersion      = VK_MAKE_VERSION(0, 0, 0),
        .apiVersion         = VK_MAKE_VERSION(1, 3, 269),
    };

    std::vector<const char*> requiredExtensions = getRequredExtensions();

    VkInstanceCreateInfo createInfo{
        .sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext            = nullptr,
        .flags            = 0,
        .pApplicationInfo = &appInfo,
        // conditionally enable the vaildation layers later
        .enabledLayerCount       = 0,
        .ppEnabledLayerNames     = nullptr,
        .enabledExtensionCount   = static_cast<uint32_t>(requiredExtensions.size()),
        .ppEnabledExtensionNames = requiredExtensions.data(),
    };

    uint32_t extensionCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> supportedExtensionProperties(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, supportedExtensionProperties.data());

#ifndef NDEBUG
    for (const VkExtensionProperties& extension : supportedExtensionProperties) {
        debugnote("Available Extension: " << extension.extensionName);
    }
#endif

#ifdef ENABLE_VALIDATION_LAYERS
    if (!checkValidationLayerSupported(validationLayers)) {
        throw std::runtime_error("validation layers not supported");
    };

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    PopulateDebugMessengerCreateInfo(debugCreateInfo);

    createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
    createInfo.pNext               = &debugCreateInfo;
#endif

    for (uint32_t i = 0; i < createInfo.enabledExtensionCount; i++) {
        if (!checkExtensionSupported(supportedExtensionProperties, createInfo.ppEnabledExtensionNames[i])) {
            std::string message("an extension was requested but not available called: ");
            message.append(createInfo.ppEnabledExtensionNames[i]);
            throw std::runtime_error(message);
        }
    }

    VkResult result;

    if ((result = vkCreateInstance(&createInfo, nullptr, instance)) != VK_SUCCESS) {
        debugerror("creating instance failed with the error: " << string_VkResult(result));
        throw std::runtime_error("failed to create instance!");
    }
}
struct Swapchain {
    VkSwapchainKHR           swapchain;
    std::vector<VkImage>     images;
    VkFormat                 format;
    VkExtent2D               extent;
    std::vector<VkImageView> imageViews;
};

struct AppData {
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    VkDebugUtilsMessengerEXT       debugMessenger;
    Window                         window{};
    VkInstance                     instance{};
    VkDevice                       device;
    VkPhysicalDevice               physicalDevice = VK_NULL_HANDLE;
    VkQueue                        graphicsQueue;
    VkQueue                        presentQueue;
    Swapchain                      swapchain;
};

VkResult CreateDebugUtilsMessengerEXT(VkInstance                                instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks*              pAllocator,
                                      VkDebugUtilsMessengerEXT*                 pMessenger) {
    auto func =
        reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
void DestroyDebugUtilsMessengerEXT(VkInstance                   instance,
                                   VkDebugUtilsMessengerEXT     pMessenger,
                                   const VkAllocationCallbacks* pAllocator) {
    auto func =
        reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        return func(instance, pMessenger, pAllocator);
    }
}

void setupDebugMessanger(VkInstance& instance, VkDebugUtilsMessengerEXT* debugMessenger) {

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    PopulateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messeneger");
    }
}

void createLogicalDevice(VkDevice&                       device,
                         const VkPhysicalDevice&         physicalDevice,
                         const std::vector<const char*>& validationLayers,
                         const std::vector<const char*>& deviceExtensions,
                         VkQueue                         graphicsQueue,
                         VkQueue                         presentQueue,
                         const VkSurfaceKHR&             surface) {

    QueueFamilyIndices                   indices = findQueueFamilies(physicalDevice, surface);
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t>                   uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount       = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures2 deviceFeatures{};

    VkDeviceCreateInfo createInfo{
        .sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
        .pQueueCreateInfos    = queueCreateInfos.data(),

        .enabledExtensionCount   = static_cast<uint32_t>(deviceExtensions.size()),
        .ppEnabledExtensionNames = deviceExtensions.data(),
        .pEnabledFeatures        = &deviceFeatures.features,
    };

#ifdef ENABLE_VALIDATION_LAYERS
    createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
#else
    createInfo.enabledLayerCount = 0;
#endif

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device");
    }
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}
void createSurface(VkInstance instance, GLFWwindow* window, VkSurfaceKHR* surface) {

    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, surface);

    if (result != VK_SUCCESS) {
        debugerror("creating window failed with the error: " << string_VkResult(result));
        throw std::runtime_error("failed to create window surface");
    }
};
void createSwapChain(const VkPhysicalDevice& physicalDevice,
                     const VkSurfaceKHR&     surface,
                     const Window&           window,
                     Swapchain&              swapchain,
                     const VkDevice&         device) {

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);
    VkSurfaceFormatKHR      surfaceFormat    = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR        presentMode      = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D              extent           = chooseSwapExtent(swapChainSupport.capabilities, window);
    uint32_t                imageCount       = swapChainSupport.capabilities.minImageCount + 1;

    if (imageCount > swapChainSupport.capabilities.maxImageCount && swapChainSupport.capabilities.maxImageCount != 0) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount    = imageCount;
    createInfo.minImageCount    = 5;
    createInfo.imageFormat      = surfaceFormat.format;
    createInfo.imageColorSpace  = surfaceFormat.colorSpace;
    createInfo.imageExtent      = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices              = findQueueFamilies(physicalDevice, surface);
    uint32_t           queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.presentFamily.value() == indices.graphicsFamily.value()) {
        createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices   = nullptr;
        debugnote("graphics and presentation queus are the same");
    } else {
        createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices   = queueFamilyIndices;
        debugnote("graphics and presentation queus are different");
    }
    createInfo.preTransform   = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode    = presentMode;
    createInfo.clipped        = VK_TRUE;
    createInfo.oldSwapchain   = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain.swapchain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swapchain");
    }

    vkGetSwapchainImagesKHR(device, swapchain.swapchain, &imageCount, nullptr);
    swapchain.images.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapchain.swapchain, &imageCount, swapchain.images.data());

    swapchain.format = surfaceFormat.format;
    swapchain.extent = extent;
}

void createImageViews(Swapchain& swapchain, const VkDevice& device) {
    swapchain.imageViews.resize(swapchain.images.size());
    for (size_t i = 0; i < swapchain.images.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image                           = swapchain.images[i];
        createInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format                          = swapchain.format;
        createInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel   = 0;
        createInfo.subresourceRange.levelCount     = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount     = 1;
        if (vkCreateImageView(device, &createInfo, nullptr, &swapchain.imageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views");
        }
    }
}

VkShaderModule createShaderModule(const VkDevice& device, const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data()); // bc this is a vector the allocator makes sure the pointer
                                                                       // already satisfies the worst case allignment requirements
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module");
    }
    return shaderModule;
}

void createGraphicsPipeline(const VkDevice& device) {
    auto vertShaderCode = readFile("build/shaders/vert.spv");
    auto fragShaderCode = readFile("build/shaders/frag.spv");

    // these are used when graphics pipeline is created, so we can destroy them right after we finish creation in this function
    VkShaderModule vertShaderModule = createShaderModule(device, vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(device, fragShaderCode);

    vkDestroyShaderModule(device, fragShaderModule, nullptr);
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
}

void cleanup(AppData& appdata) {
    for (auto& imageView : appdata.swapchain.imageViews) {
        vkDestroyImageView(appdata.device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(appdata.device, appdata.swapchain.swapchain, nullptr);

    vkDestroyDevice(appdata.device, nullptr);
    vkDestroySurfaceKHR(appdata.instance, appdata.window.surface, nullptr);

#ifdef ENABLE_VALIDATION_LAYERS
    DestroyDebugUtilsMessengerEXT(appdata.instance, appdata.debugMessenger, nullptr);
#endif
    vkDestroyInstance(appdata.instance, nullptr);

    glfwDestroyWindow(appdata.window.windowPointer);
    glfwTerminate();
}

int main() {
    debugnote("hello world this is kinda weird");
    glfwInit();

    AppData appdata{};
    initializeWindow(appdata.window.windowPointer, 800, 600, "window");
    createVkInstance(&appdata.instance, appdata.validationLayers);
#ifdef ENABLE_VALIDATION_LAYERS
    setupDebugMessanger(appdata.instance, &appdata.debugMessenger);
#endif
    createSurface(appdata.instance, appdata.window.windowPointer, &appdata.window.surface);
    pickPhysicalDevice(appdata.instance, appdata.physicalDevice, appdata.window.surface, appdata.deviceExtensions);
    createLogicalDevice(appdata.device,
                        appdata.physicalDevice,
                        appdata.validationLayers,
                        appdata.deviceExtensions,
                        appdata.graphicsQueue,
                        appdata.presentQueue,
                        appdata.window.surface);

    createSwapChain(appdata.physicalDevice, appdata.window.surface, appdata.window, appdata.swapchain, appdata.device);
    createImageViews(appdata.swapchain, appdata.device);
    createGraphicsPipeline();

    debugnote("graphics queue: " << appdata.graphicsQueue);
    debugnote("present queue: " << appdata.presentQueue);

    while (!glfwWindowShouldClose(appdata.window.windowPointer)) {
        glfwPollEvents();
    }
    cleanup(appdata);
}
