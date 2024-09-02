#pragma once
#include <glm/fwd.hpp>
#include <vector>

#include "../engine/enginedata.h"
#include "../engine/vertex.h"

struct Model {
    std::vector<Vertex>   vertices{};
    std::vector<uint32_t> indices{};
    glm::vec3             position{};

    Buffer vertexBuffer{};
    Buffer indexBuffer{};
};
struct GameData {
    std::vector<Model> models{};
};
