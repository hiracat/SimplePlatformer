#include <GLFW/glfw3.h>
#include <cstring>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "../../compilesettings.h"

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

std::vector<const char*> getRequredExtensions() {
    // NOTE: get the required extensions by glfw
    uint32_t                 glfwExtensionCount;
    const char**             glfwExtensionNames = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> requiredExtensions(glfwExtensionNames, glfwExtensionNames + glfwExtensionCount);

#ifdef ENABLE_VALIDATION_LAYERS
    requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    return requiredExtensions;
}
