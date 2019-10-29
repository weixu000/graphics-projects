#include "Transform.h"

Transform::Transform(const glm::mat4 &m)
        : model(m) {

}

void Transform::draw(const glm::mat4 &world) {
    auto m = world * model;
    for (auto &n:children) {
        n->draw(m);
    }
}

void Transform::addChild(NodePtr child) {
    children.push_back(std::move(child));
}
