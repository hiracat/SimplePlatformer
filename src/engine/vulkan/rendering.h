#pragma once
#include "../../game/gamedata.h"
#include "../enginedata.h"
#include <chrono>

void drawFrame(Data& data, GameData& gamedata, std::chrono::high_resolution_clock::time_point startTime);
