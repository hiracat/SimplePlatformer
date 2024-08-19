#pragma once
#include <vector>

#include "../engine/vertex.h"

struct Model {
    std::vector<Vertex>   vertices;
    std::vector<uint32_t> indices;
};
struct GameData {
    std::vector<Model> models;
};
