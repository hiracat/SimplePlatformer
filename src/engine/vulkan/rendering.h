#pragma once
#include "../engine.h"
#include <chrono>

void drawFrame(EngineData& data, const std::vector<Model>& models, std::chrono::high_resolution_clock::time_point startTime);
