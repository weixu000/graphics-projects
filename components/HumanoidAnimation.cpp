#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "HumanoidAnimation.h"
#include "../Time.h"

HumanoidAnimation::HumanoidAnimation()
        : headControl(std::make_shared<Transform>()),
          leftArmControl(std::make_shared<Transform>()),
          rightArmControl(std::make_shared<Transform>()),
          leftLegControl(std::make_shared<Transform>()),
          rightLegControl(std::make_shared<Transform>()) {

}

void HumanoidAnimation::update() {
    static auto deg = 0.0f;
    static auto dir = true;
    const auto speed = 3.0f;

    if (dir) {
        deg += speed * Time::delta();
    } else {
        deg -= speed * Time::delta();
    }

    if (deg > glm::pi<float>() / 4) {
        deg = glm::pi<float>() / 4;
        dir = false;
    } else if (deg < -glm::pi<float>() / 4) {
        deg = -glm::pi<float>() / 4;
        dir = true;
    }

    leftArmControl->model = glm::rotate(-deg, glm::vec3(1.0f, 0.0f, 0.0f));
    rightArmControl->model = glm::rotate(deg, glm::vec3(1.0f, 0.0f, 0.0f));

    leftLegControl->model = glm::rotate(deg, glm::vec3(1.0f, 0.0f, 0.0f));
    rightLegControl->model = glm::rotate(-deg, glm::vec3(1.0f, 0.0f, 0.0f));
}
