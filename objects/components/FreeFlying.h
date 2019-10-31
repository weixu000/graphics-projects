#ifndef FREEFLYING_H
#define FREEFLYING_H

#include "Transform.h"

class FreeFlying : public Transform {
public:
    using Transform::Transform;

    // x,y should be normalized
    void startRotate(float x, float y);

    // x,y should be normalized
    void rotate(float x, float y);

    void stopRotate();

    enum class MoveDirection {
        Forward,
        Backward,
        Left,
        Right,
    };

    void move(MoveDirection direction);

private:
    bool startedRotate = false;
    glm::vec2 initialMouse;
};


#endif //FREEFLYING_H
