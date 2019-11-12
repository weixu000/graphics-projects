#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>
#include <memory>
#include <list>

#include "../components/Component.h"
#include "../components/Transform.h"

class Node {
public:
    explicit Node(const glm::mat4 &t = glm::mat4(1.0f));

    explicit Node(std::shared_ptr<Transform> t);

    Node(Node &&) = default;

    Node &operator=(Node &&) = default;

    virtual ~Node() = default;

    virtual void draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye);

    virtual void update();

    virtual bool cull(const glm::mat4 &view_proj);

    bool culled() const { return _culled; }

    using NodePtr = std::unique_ptr<Node>;

    Node *addChild(NodePtr child);

    Node *parent() const { return _parent; }

    using ComponentPtr = std::shared_ptr<Component>;

    void addComponent(ComponentPtr component);

    std::shared_ptr<Transform> transform;

    // deep copy children
    // shallow copy components
    // initialize _culled, _parent
    virtual std::unique_ptr<Node> copy();

protected:
    bool _culled = false;

    Node *_parent = nullptr;

    std::list<NodePtr> children;
    std::list<ComponentPtr> components;

    // do actual copy with allocation
    void doCopy(Node *dup);
};


#endif //NODE_H
