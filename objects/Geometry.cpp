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

void Geometry::cull(const glm::mat4 &view_proj) {
    auto bb = boundingBox();
    glm::mat4 blow_p(glm::vec4(bb.vertices[0], 1.0f),
                     glm::vec4(bb.vertices[1], 1.0f),
                     glm::vec4(bb.vertices[2], 1.0f),
                     glm::vec4(bb.vertices[3], 1.0f));

    glm::mat4 upper_p(glm::vec4(bb.vertices[4], 1.0f),
                      glm::vec4(bb.vertices[5], 1.0f),
                      glm::vec4(bb.vertices[6], 1.0f),
                      glm::vec4(bb.vertices[7], 1.0f));
    for (auto i = 0; i < 3; ++i) {
        if ((upper_p[0][i] > upper_p[0].w && upper_p[1][i] > upper_p[1].w &&
             upper_p[2][i] > upper_p[2].w && upper_p[3][i] > upper_p[1].w) ||
            (upper_p[0][i] < -upper_p[0].w && upper_p[1][i] < -upper_p[1].w &&
             upper_p[2][i] < -upper_p[2].w && upper_p[3][i] < -upper_p[1].w)) {
            _culled = true;
            return;
        }
    }
    _culled = false;
}
