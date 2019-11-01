#include "Node.h"

Node::Node(std::shared_ptr<Transform> t)
        : transform(std::move(t)) {
}

Node::Node(const glm::mat4 &t)
        : transform(std::make_shared<Transform>(t)) {

}

void Node::draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) {
    auto m = world * transform->model;

    for (auto &n:components) {
        n->draw(m, projection, view, eye);
    }

    for (auto &n:children) {
        if (!n->culled()) {
            n->draw(m, projection, view, eye);
        }
    }
}

void Node::update() {
    transform->update();

    for (auto &n:components) {
        n->update();
    }

    for (auto &n:children) {
        n->update();
    }
}

bool Node::cull(const glm::mat4 &view_proj) {
    auto m = view_proj * transform->model;
    size_t num_culled = 0;

    for (auto &n:components) {
        if (n->cull(m)) {
            ++num_culled;
        }
    }

    for (auto &n:children) {
        if (n->cull(m)) {
            ++num_culled;
        }
    }
    _culled = num_culled == components.size() + children.size();
    return _culled;
}

Node *Node::addChild(NodePtr child) {
    assert(!child->_parent);
    child->_parent = this;
    children.push_back(std::move(child));
    return children.back().get();
}

void Node::addComponent(Node::ComponentPtr component) {
    components.push_back(std::move(component));
}

std::unique_ptr<Node> Node::copy() {
    auto ret = std::make_unique<Node>();
    doCopy(ret.get());
    return ret;
}

void Node::doCopy(Node *dup) {
    dup->transform = transform;
    for (auto &n:children) {
        dup->addChild(n->copy());
    }
    for (auto &n:components) {
        dup->components.push_back(n);
    }
}
