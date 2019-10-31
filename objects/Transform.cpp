#include "Transform.h"

Transform::Transform(const glm::mat4 &m)
        : model(m) {}

void Transform::draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) {
    auto m = world * model;
    for (auto &n:children) {
        if (!n->culled()) {
            n->draw(m, projection, view, eye);
        }
    }
}

void Transform::addChild(NodePtr child) {
    children.push_back(std::move(child));
}

void Transform::update() {
    for (auto &n:children) {
        n->update();
    }
}

void Transform::cull(const glm::mat4 &view_proj) {
    auto m = view_proj * model;
    std::list<NodePtr>::size_type num_culled = 0;
    for (auto &n:children) {
        n->cull(m);
        if (n->culled()) {
            ++num_culled;
        }
    }
    _culled = num_culled == children.size();
}
