#ifndef PROJECT3_TRANSFORM_H
#define PROJECT3_TRANSFORM_H

#include <glm/glm.hpp>
#include <memory>
#include <list>

#include "Component.h"

class Transform : public Component {
public:
    explicit Transform(const glm::mat4 &m = glm::mat4(1.0f));

    glm::mat4 model;
};


#endif //PROJECT3_TRANSFORM_H
