#ifndef COMPONENT_H
#define COMPONENT_H

#include <glm/glm.hpp>


class Component {
public:
    virtual ~Component() = default;

    virtual void
    draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) {}

    virtual void update() {};

    virtual bool cull(const glm::mat4 &view_proj) { return false; }
};


#endif //COMPONENT_H
