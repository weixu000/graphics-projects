#ifndef PROJECT3_TRANSFORM_H
#define PROJECT3_TRANSFORM_H

#include <glm/glm.hpp>
#include <memory>
#include <list>

#include "Node.h"

class Transform : public Node {
public:
    using NodePtr = std::shared_ptr<Node>;

    Transform() = default;

    explicit Transform(const glm::mat4 &m);

    void draw(const glm::mat4 &world) override;

    void addChild(NodePtr child);

protected:
    glm::mat4 model{1.0f};
    std::list<NodePtr> children;
};


#endif //PROJECT3_TRANSFORM_H
