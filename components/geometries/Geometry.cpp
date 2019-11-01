#include <glm/gtx/component_wise.hpp>

#include "Geometry.h"

AABB::AABB(const glm::vec3 &minVal, const glm::vec3 &maxVal) {
    vertices[0] = vertices[1] = vertices[2] = vertices[3] = minVal;
    vertices[1].z = vertices[2].z = maxVal.z;
    vertices[2].x = vertices[3].x = maxVal.x;

    vertices[4] = vertices[0];
    vertices[5] = vertices[1];
    vertices[6] = vertices[2];
    vertices[7] = vertices[3];
    vertices[4].y = vertices[5].y = vertices[6].y = vertices[7].y = maxVal.y;
}

bool Geometry::cull(const glm::mat4 &view_proj) {
    auto bb = boundingBox();
    std::array<glm::vec4, 8> vertices;
    for (size_t i = 0; i < 8; ++i) {
        vertices[i] = view_proj * glm::vec4(bb.vertices[i], 1.0f);
    }
    for (size_t i = 0; i < 3; ++i) {
        size_t j;
        for (j = 0; j < 8 && vertices[j][i] > vertices[j].w; ++j) {}
        if (j == 8) {
            return true;
        }
        for (j = 0; j < 8 && vertices[j][i] < -vertices[j].w; ++j) {}
        if (j == 8) {
            return true;
        }
    }
    return false;
}
