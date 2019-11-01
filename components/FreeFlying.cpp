#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include "FreeFlying.h"
#include "../Time.h"

void FreeFlying::startRotate(float x, float y) {
    startedRotate = true;
    initialMouse = glm::vec2(x, y);
}

void FreeFlying::stopRotate() {
    startedRotate = false;
}

void FreeFlying::rotate(float x, float y) {
    if (startedRotate) {
        auto delta_angle = 1.0f * (glm::vec2(x, y) - initialMouse);
        delta_angle = glm::clamp(delta_angle, -glm::pi<float>() / 2, glm::pi<float>() / 2);
        auto delta_rot = glm::eulerAngleXY(delta_angle.y, delta_angle.x);
        model = model * delta_rot;
        initialMouse = glm::vec2(x, y);
    }
}

void FreeFlying::update() {
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
