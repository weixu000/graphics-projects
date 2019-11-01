#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "HumanoidAnimation.h"
#include "../../Time.h"

HumanoidAnimation::HumanoidAnimation()
        : headControl(std::make_shared<Transform>()),
          leftArmControl(std::make_shared<Transform>()),
          rightArmControl(std::make_shared<Transform>()),
          leftLegControl(std::make_shared<Transform>()),
          rightLegControl(std::make_shared<Transform>()) {

}

void HumanoidAnimation::update() {
    static float left_leg = 0.0f, right_leg = 0.0f;
    static auto dir = true;

    if (dir) {
        left_leg += 1.0f * Time::delta();
        right_leg += 1.0f * Time::delta();
    } else {
        left_leg -= 1.0f * Time::delta();
        right_leg -= 1.0f * Time::delta();
    }

    if (left_leg > glm::pi<float>() / 4) {
        left_leg = glm::pi<float>() / 4;
        dir = false;
    } else if (left_leg < -glm::pi<float>() / 4) {
        left_leg = -glm::pi<float>() / 4;
        dir = true;
    }

    leftArmControl->model = glm::rotate(right_leg, glm::vec3(1.0f, 0.0f, 0.0f));
    rightArmControl->model = glm::rotate(left_leg, glm::vec3(-1.0f, 0.0f, 0.0f));

    leftLegControl->model = glm::rotate(left_leg, glm::vec3(1.0f, 0.0f, 0.0f));
    rightLegControl->model = glm::rotate(right_leg, glm::vec3(-1.0f, 0.0f, 0.0f));
}
