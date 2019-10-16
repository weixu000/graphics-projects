#ifndef TRACKBALL_H
#define TRACKBALL_H

#include <glm/glm.hpp>

class Trackball {
public:
    void start(glm::mat4 *m, float x, float y);

    void move(float x, float y);

    void stop();

    void scale(glm::mat4 *m, float offset);

private:
    glm::mat4 *mat = nullptr, origin;
    glm::mat4 dragRot{1.0f};
    glm::vec3 startDrag;
};


#endif // TRACKBALL_H
