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
