#include "Trackball.h"
#include "Window.h"

void Trackball::start(glm::mat4 *m, float x, float y) {
    startDrag.x = 2 * x / Window::width - 1;
    startDrag.y = -(2 * y / Window::height - 1);
    auto z2 = 1 - startDrag.x * startDrag.x - startDrag.y * startDrag.y;
    if (z2 > 0) {
        mat = m;
        startDrag.z = glm::sqrt(z2);
        origin = *mat;
    }
}

void Trackball::move(float x, float y) {
    if (mat) {
        glm::vec3 cur(2 * x / Window::width - 1, -(2 * y / Window::height - 1), 0);
        auto z2 = 1 - cur.x * cur.x - cur.y * cur.y;
        if (z2 > 0) {
            cur.z = glm::sqrt(z2);
            auto rad = glm::acos(glm::dot(startDrag, cur));
            auto axis = glm::cross(startDrag, cur);
            dragRot = glm::mat4_cast(glm::angleAxis(rad, axis));
            *mat = dragRot * origin;
        }
    }
}

void Trackball::stop() {
    if (mat) {
        *mat = dragRot * origin;
        mat = nullptr;
        dragRot = glm::mat4(1.0);
    }
}

void Trackball::scale(glm::mat4 *m, float offset) {
    auto s = glm::scale(glm::vec3(glm::max(0.0f, 1 + 0.1f * offset)));
    *m = s * *m;
    if (mat == m) {
        origin = s * origin;
    }
}
