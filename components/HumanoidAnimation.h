#ifndef HUMANOIDANIMATION_H
#define HUMANOIDANIMATION_H

#include "Component.h"
#include "Transform.h"


class HumanoidAnimation : public Component {
public:
    HumanoidAnimation();

    void update() override;

    std::shared_ptr<Transform> headControl, leftArmControl, rightArmControl, leftLegControl, rightLegControl;
};


#endif //HUMANOIDANIMATION_H
