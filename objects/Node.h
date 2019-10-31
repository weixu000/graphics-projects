#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>
#include <memory>
#include <list>

#include "components/Component.h"
#include "components/Transform.h"

class Node {
public:
    explicit Node(const glm::mat4 &t = glm::mat4(1.0f));

    explicit Node(std::shared_ptr<Transform> t);

    virtual ~Node() = default;

    virtual void draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye);

    virtual void update();

    virtual bool cull(const glm::mat4 &view_proj);

    bool culled() const { return _culled; }

    Node *addChild(Node child);

    using ComponentPtr = std::shared_ptr<Component>;

    void addComponent(ComponentPtr component);

    std::shared_ptr<Transform> transform;

protected:
    bool _culled = false;

    std::list<Node> children;
    std::list<ComponentPtr> components;
};


#endif //NODE_H
