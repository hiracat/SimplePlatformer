#pragma once
#include "enginedata.h"
#include "vertex.h"
#include <vector>
#include <vulkan/vulkan_core.h>

void initEngine(EngineData& enginedata, const std::vector<Vertex>& renderData);
