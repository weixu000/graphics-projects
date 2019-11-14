#ifndef CONSTRAINTANIMATOR_H
#define CONSTRAINTANIMATOR_H

#include "Component.h"
#include "Transform.h"
#include "geometries/BezierCurve.h"

class ConstraintAnimator : public Component {
public:
    explicit ConstraintAnimator(std::shared_ptr<BezierCurve> t, float speed = 1.0f);

    void update() override;

    std::shared_ptr<Transform> control;

    bool pause = false;

private:
    std::shared_ptr<BezierCurve> track;

    void set();

    float t = 0.0f;
    float speed = 10.0f;
};


#endif //CONSTRAINTANIMATOR_H
