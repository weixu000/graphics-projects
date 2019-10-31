#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include "FreeFlying.h"
#include "../../Time.h"

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

void FreeFlying::move(FreeFlying::MoveDirection direction) {
    const auto sensitivity = 5.0f * Time::delta();
    switch (direction) {
        case MoveDirection::Forward:
            model = glm::translate(-sensitivity * glm::vec3(model[2])) * model;
            break;
        case MoveDirection::Backward:
            model = glm::translate(sensitivity * glm::vec3(model[2])) * model;
            break;
        case MoveDirection::Left:
            model = glm::translate(-sensitivity * glm::vec3(model[0])) * model;
            break;
        case MoveDirection::Right:
            model = glm::translate(sensitivity * glm::vec3(model[0])) * model;
            break;
    }
}
