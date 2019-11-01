#include <glm/gtx/euler_angles.hpp>

#include "Robot.h"
#include "Wireframe.h"

Robot::Robot(std::shared_ptr<Transform> headCtl,
             std::shared_ptr<Transform> leftArmCtl,
             std::shared_ptr<Transform> rightArmCtl,
             std::shared_ptr<Transform> leftLegCtl,
             std::shared_ptr<Transform> rightLegCtl)
        : antenna(std::make_shared<Mesh>(Mesh::fromObjFile("meshes/Robot-parts-2018/antenna_s.obj"))),
          eyeball(std::make_shared<Mesh>(Mesh::fromObjFile("meshes/Robot-parts-2018/eyeball_s.obj"))),
          head(std::make_shared<Mesh>(Mesh::fromObjFile("meshes/Robot-parts-2018/head_s.obj"))),
          limb(std::make_shared<Mesh>(Mesh::fromObjFile("meshes/Robot-parts-2018/limb_s.obj"))),
          body(std::make_shared<Mesh>(Mesh::fromObjFile("meshes/Robot-parts-2018/body_s.obj"))) {
    root.addComponent(body);
    root.addComponent(std::make_shared<Wireframe>(Wireframe::fromAABB(boundingBox())));

    initHead(std::move(headCtl));
    initArms(std::move(leftArmCtl), std::move(rightArmCtl));
    initLegs(std::move(leftLegCtl), std::move(rightLegCtl));
}

void Robot::initHead(std::shared_ptr<Transform> headCtl) {
    auto antenna_m = glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::vec3(0.3f));
    auto antenna_l_m = std::make_unique<Node>(
            glm::rotate(glm::pi<float>() / 4, glm::vec3(0.0f, 0.0f, 1.0f)) * antenna_m);
    antenna_l_m->addComponent(antenna);
    auto antenna_r_m = std::make_unique<Node>(
            glm::rotate(-glm::pi<float>() / 4, glm::vec3(0.0f, 0.0f, 1.0f)) * antenna_m);
    antenna_r_m->addComponent(antenna);

    auto eyeball_m = glm::eulerAngleX(glm::pi<float>() * 0.4f) * glm::translate(glm::vec3(0.0f, 1.0f, 0.0f));
    auto eyeball_l_m = std::make_unique<Node>(glm::eulerAngleY(glm::pi<float>() * 0.15f) * eyeball_m);
    eyeball_l_m->addComponent(eyeball);
    auto eyeball_r_m = std::make_unique<Node>(glm::eulerAngleY(glm::pi<float>() * -0.15f) * eyeball_m);
    eyeball_r_m->addComponent(eyeball);

    auto control_node = std::make_unique<Node>(std::move(headCtl));
    control_node->addComponent(head);
    control_node->addChild(std::move(antenna_l_m));
    control_node->addChild(std::move(antenna_r_m));
    control_node->addChild(std::move(eyeball_l_m));
    control_node->addChild(std::move(eyeball_r_m));

    auto head_m = std::make_unique<Node>(glm::translate(glm::vec3(0.0f, 1.2f, 0.0f)));
    head_m->addChild(std::move(control_node));
    root.addChild(std::move(head_m));
}

void Robot::initArms(std::shared_ptr<Transform> leftCtl, std::shared_ptr<Transform> rightCtl) {
    auto arm_m = std::make_shared<Transform>(
            glm::translate(glm::vec3(0.0f, -0.8f, 0.0f)) * glm::scale(glm::vec3(1.0f, 2.0f, 1.0f)));

    auto arm = std::make_unique<Node>(arm_m);
    arm->addComponent(limb);
    auto left_node = std::make_unique<Node>(std::move(leftCtl));
    left_node->addChild(std::move(arm));
    auto arm_l_b = std::make_unique<Node>(glm::translate(glm::vec3(-1.4f, 0.6f, 0.0f)));
    arm_l_b->addChild(std::move(left_node));

    arm = std::make_unique<Node>(arm_m);
    arm->addComponent(limb);
    auto right_node = std::make_unique<Node>(std::move(rightCtl));
    right_node->addChild(std::move(arm));
    auto arm_r_b = std::make_unique<Node>(glm::translate(glm::vec3(1.4f, 0.6f, 0.0f)));
    arm_r_b->addChild(std::move(right_node));

    root.addChild(std::move(arm_l_b));
    root.addChild(std::move(arm_r_b));
}

void Robot::initLegs(std::shared_ptr<Transform> leftCtl, std::shared_ptr<Transform> rightCtl) {
    auto leg_m = std::make_shared<Transform>(
            glm::translate(glm::vec3(0.0f, -0.7f, 0.0f)));

    auto leg = std::make_unique<Node>(leg_m);
    leg->addComponent(limb);
    auto left_node = std::make_unique<Node>(std::move(leftCtl));
    left_node->addChild(std::move(leg));
    auto leg_l_m = std::make_unique<Node>(glm::translate(glm::vec3(-0.5f, -1.0f, 0.0f)));
    leg_l_m->addChild(std::move(left_node));

    leg = std::make_unique<Node>(leg_m);
    leg->addComponent(limb);
    auto right_node = std::make_unique<Node>(std::move(rightCtl));
    right_node->addChild(std::move(leg));
    auto leg_r_m = std::make_unique<Node>(glm::translate(glm::vec3(0.5f, -1.0f, 0.0f)));
    leg_r_m->addChild(std::move(right_node));

    root.addChild(std::move(leg_l_m));
    root.addChild(std::move(leg_r_m));
}

void Robot::draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) {
    root.draw(world, projection, view, eye);
}

void Robot::useShader(const std::shared_ptr<Shader> &s) {
    antenna->useShader(s);
    eyeball->useShader(s);
    head->useShader(s);
    limb->useShader(s);
    body->useShader(s);
}
