#pragma once
#include <vulkan/vulkan.h>

#include "enginedata.h"
#include <GLFW/glfw3.h>

void initializeWindow(VkInstance instance, WindowResources& windowData, bool* frameBufferResized);
