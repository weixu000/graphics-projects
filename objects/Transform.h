#ifndef PROJECT3_TRANSFORM_H
#define PROJECT3_TRANSFORM_H

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Node.h"

class Transform : public Node {
public:
    Transform() = default;

    explicit Transform(const glm::mat4 &m);

    void draw(const glm::mat4 &world) override;

    void addChild(const std::shared_ptr<Node> &child);

protected:
    glm::mat4 model{1.0f};
    std::vector<std::shared_ptr<Node>> children;
};


#endif //PROJECT3_TRANSFORM_H
