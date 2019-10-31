#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>


class Node {
public:
    virtual ~Node() = default;

    virtual void
    draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) = 0;

    virtual void update() {};

    virtual void cull(const glm::mat4 &view_proj) = 0;

    bool culled() const { return _culled; }

protected:
    bool _culled = false;
};


#endif //NODE_H
