#include <glm/gtx/euler_angles.hpp>

#include "Robot.h"
#include "../../../Time.h"
#include "Wireframe.h"

Robot::Robot()
        : root(std::make_shared<Transform>()),
          antenna(std::make_shared<Mesh>(Mesh::fromObjFile("meshes/Robot-parts-2018/antenna_s.obj"))),
          eyeball(std::make_shared<Mesh>(Mesh::fromObjFile("meshes/Robot-parts-2018/eyeball_s.obj"))),
          head(std::make_shared<Mesh>(Mesh::fromObjFile("meshes/Robot-parts-2018/head_s.obj"))),
          limb(std::make_shared<Mesh>(Mesh::fromObjFile("meshes/Robot-parts-2018/limb_s.obj"))),
          body(std::make_shared<Mesh>(Mesh::fromObjFile("meshes/Robot-parts-2018/body_s.obj"))) {

    root.addComponent(body);
    root.addComponent(std::make_shared<Wireframe>(Wireframe::fromAABB(boundingBox())));

    initHead();
    initArms();
    initLegs();
}

void Robot::initHead() {
    auto antenna_m = glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::vec3(0.3f));
    Node antenna_l_m(
            glm::rotate(glm::pi<float>() / 4, glm::vec3(0.0f, 0.0f, 1.0f)) * antenna_m);
    antenna_l_m.addComponent(antenna);
    Node antenna_r_m(
            glm::rotate(-glm::pi<float>() / 4, glm::vec3(0.0f, 0.0f, 1.0f)) * antenna_m);
    antenna_r_m.addComponent(antenna);

    auto eyeball_m = glm::eulerAngleX(glm::pi<float>() * 0.4f) * glm::translate(glm::vec3(0.0f, 1.0f, 0.0f));
    Node eyeball_l_m(glm::eulerAngleY(glm::pi<float>() * 0.15f) * eyeball_m);
    eyeball_l_m.addComponent(eyeball);
    Node eyeball_r_m(glm::eulerAngleY(glm::pi<float>() * -0.15f) * eyeball_m);
    eyeball_r_m.addComponent(eyeball);

    headControl = std::make_shared<Transform>();
    Node control_node(headControl);
    control_node.addComponent(head);
    control_node.addChild(antenna_l_m);
    control_node.addChild(antenna_r_m);
    control_node.addChild(eyeball_l_m);
    control_node.addChild(eyeball_r_m);

    Node head_m(glm::translate(glm::vec3(0.0f, 1.2f, 0.0f)));
    head_m.addChild(control_node);
    root.addChild(head_m);
}

void Robot::initArms() {
    Node arm(
            glm::translate(glm::vec3(0.0f, -0.8f, 0.0f)) * glm::scale(glm::vec3(1.0f, 2.0f, 1.0f)));
    arm.addComponent(limb);

    leftArmControl = std::make_shared<Transform>();
    Node left_node(leftArmControl);
    left_node.addChild(arm);
    Node arm_l_b(glm::translate(glm::vec3(-1.4f, 0.6f, 0.0f)));
    arm_l_b.addChild(left_node);

    rightArmControl = std::make_shared<Transform>();
    Node right_node(rightArmControl);
    right_node.addChild(arm);
    Node arm_r_b(glm::translate(glm::vec3(1.4f, 0.6f, 0.0f)));
    arm_r_b.addChild(right_node);

    root.addChild(arm_l_b);
    root.addChild(arm_r_b);
}

void Robot::initLegs() {
    Node leg_m(glm::translate(glm::vec3(0.0f, -0.7f, 0.0f)));
    leg_m.addComponent(limb);

    leftLegControl = std::make_shared<Transform>();
    Node left_node(leftLegControl);
    left_node.addChild(leg_m);
    Node leg_l_m(glm::translate(glm::vec3(-0.5f, -1.0f, 0.0f)));
    leg_l_m.addChild(left_node);

    rightLegControl = std::make_shared<Transform>();
    Node right_node(rightLegControl);
    right_node.addChild(leg_m);
    Node leg_r_m(glm::translate(glm::vec3(0.5f, -1.0f, 0.0f)));
    leg_r_m.addChild(right_node);

    root.addChild(leg_l_m);
    root.addChild(leg_r_m);
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

void Robot::update() {
    static float left_leg = 0.0f, right_leg = 0.0f;
    static auto dir = true;

    if (dir) {
        left_leg += 1.0f * Time::delta();
        right_leg += 1.0f * Time::delta();
    } else {
        left_leg -= 1.0f * Time::delta();
        right_leg -= 1.0f * Time::delta();
    }

    if (left_leg > glm::pi<float>() / 4) {
        left_leg = glm::pi<float>() / 4;
        dir = false;
    } else if (left_leg < -glm::pi<float>() / 4) {
        left_leg = -glm::pi<float>() / 4;
        dir = true;
    }

    leftArmControl->model = glm::rotate(right_leg, glm::vec3(1.0f, 0.0f, 0.0f));
    rightArmControl->model = glm::rotate(left_leg, glm::vec3(-1.0f, 0.0f, 0.0f));

    leftLegControl->model = glm::rotate(left_leg, glm::vec3(1.0f, 0.0f, 0.0f));
    rightLegControl->model = glm::rotate(right_leg, glm::vec3(-1.0f, 0.0f, 0.0f));
}
