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

    void draw(const glm::mat4 &world) override;

    void update() override;

    void addChild(NodePtr child);

protected:
    std::list<NodePtr> children;
};


#endif //PROJECT3_TRANSFORM_H
