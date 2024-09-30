#pragma once
#include "engine.h"
#include <vulkan/vulkan_core.h>

void createModels(EngineData* engineData, std::vector<Model>* models);
void addModel(EngineData* engineData, Model* model);
