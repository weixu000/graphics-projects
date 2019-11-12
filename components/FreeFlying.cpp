#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include "FreeFlying.h"
#include "../Time.h"

void FreeFlying::startRotate(const glm::vec3 &dir) {
    startedRotate = !freeze;
    initial = dir;
}

void FreeFlying::stopRotate() {
    startedRotate = false;
}

void FreeFlying::rotate(const glm::vec3 &dir) {
    if (startedRotate) {
        auto dot = glm::clamp(glm::dot(initial, dir), -1.0f, 1.0f);
        auto rad = -glm::acos(dot);
        auto axis = glm::cross(initial, dir);
        if (glm::length(axis) == 0) {
            return;
        }
        auto rot = glm::rotate(rad, axis);
        model = model * rot;
        initial = dir;
    }
}

void FreeFlying::update() {
    if (freeze) return;
    const auto sensitivity = 10.0f * Time::delta();
    if (forward) {
        auto vec = local ? glm::vec3(model[2]) : glm::vec3(0.0f, 0.0f, 1.0f);
        model = glm::translate(-sensitivity * vec) * model;
    }
    if (backward) {
        auto vec = local ? glm::vec3(model[2]) : glm::vec3(0.0f, 0.0f, 1.0f);
        model = glm::translate(sensitivity * vec) * model;
    }
    if (left) {
        auto vec = local ? glm::vec3(model[0]) : glm::vec3(1.0f, 0.0f, 0.0f);
        model = glm::translate(-sensitivity * vec) * model;
    }
    if (right) {
        auto vec = local ? glm::vec3(model[0]) : glm::vec3(1.0f, 0.0f, 0.0f);
        model = glm::translate(sensitivity * vec) * model;
    }
    if (up) {
        auto vec = local ? glm::vec3(model[1]) : glm::vec3(0.0f, 1.0f, 0.0f);
        model = glm::translate(sensitivity * vec) * model;
    }
    if (down) {
        auto vec = local ? glm::vec3(model[1]) : glm::vec3(0.0f, 1.0f, 0.0f);
        model = glm::translate(-sensitivity * vec) * model;
    }
}

glm::vec3 FreeFlying::windowCoordToCamDir(float x, float y, int width, int height, const glm::mat4 &proj) {
    auto viewport = glm::vec4(0.0f, 0.0f, width, height);
    auto win_coord = glm::vec3(x, height - 1 - y, 0);
    return glm::normalize(glm::unProject(win_coord, glm::mat4(1.0f), proj, viewport));
}
