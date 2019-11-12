#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "ConstraintAnimator.h"
#include "../Time.h"

ConstraintAnimator::ConstraintAnimator(std::shared_ptr<BezierCurve> t, float speed)
        : track(std::move(t)), control(std::make_shared<Transform>()), speed(speed) {
    set();
}

void ConstraintAnimator::update() {
    t += speed * Time::delta() / glm::length(track->derivative(t));
    set();
}

void ConstraintAnimator::set() {
    auto z = glm::vec3(control->model[2]);
    auto z_ = glm::normalize(track->derivative(t));
    control->model = glm::translate(track->position(t)) *
                     glm::mat4(glm::rotation(z, z_)) * glm::mat4(glm::mat3(control->model));
}
