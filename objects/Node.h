#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>


class Node {
public:
    virtual ~Node() = default;

    virtual void
    draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) = 0;

    virtual void update() {};
};


#endif //NODE_H
