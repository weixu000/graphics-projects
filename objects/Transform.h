#ifndef PROJECT3_TRANSFORM_H
#define PROJECT3_TRANSFORM_H

#include <glm/glm.hpp>
#include <memory>
#include <list>

#include "Node.h"

class Transform : public Node {
public:
    using NodePtr = std::shared_ptr<Node>;

    explicit Transform(const glm::mat4 &m = glm::mat4(1.0f));

    glm::mat4 model;

    void
    draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) override;

    void update() override;

    void cull(const glm::mat4 &view_proj) override;

    void addChild(NodePtr child);

protected:
    std::list<NodePtr> children;
};


#endif //PROJECT3_TRANSFORM_H
