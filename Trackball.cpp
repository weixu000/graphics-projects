#include "Trackball.h"
#include "Window.h"

void Trackball::start(float x, float y) {
    initial = viewportToTrackball(x, y);
    started = true;
}

glm::mat4 Trackball::move(float x, float y) {
    if (started) {
        auto cur = viewportToTrackball(x, y);
        auto rad = glm::acos(glm::dot(initial, cur));
        auto axis = glm::cross(initial, cur);
        auto next = glm::rotate(rad, axis);
        auto delta = next * glm::transpose(orientation);
        orientation = next;
        return delta;
    } else {
        return glm::mat4(1.0f);
    }
}

void Trackball::stop() {
    started = false;
    orientation = glm::mat4(1.0f);
}

glm::mat4 Trackball::scale(float offset) {
    return glm::scale(glm::vec3(glm::max(0.0f, 1 + 0.1f * offset)));
}

glm::vec3 Trackball::viewportToTrackball(float x, float y) {
    glm::vec3 ret(2 * x / Window::width - 1,
                  -(2 * y / Window::height - 1),
                  0.0f);
    auto z2 = 1 - ret.x * ret.x - ret.y * ret.y;
    if (z2 >= 0) {
        ret.z = glm::sqrt(z2);
        return ret;
    } else {
        return glm::normalize(ret);
    }
}