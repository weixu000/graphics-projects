#include <glm/gtx/euler_angles.hpp>

#include "Robot.h"
#include "../Time.h"
#include "Wireframe.h"

Robot::Robot()
        : root(std::make_shared<Transform>()),
          antenna(std::make_shared<Mesh>(Mesh::fromObjFile("meshes/Robot-parts-2018/antenna_s.obj"))),
          eyeball(std::make_shared<Mesh>(Mesh::fromObjFile("meshes/Robot-parts-2018/eyeball_s.obj"))),
          head(std::make_shared<Mesh>(Mesh::fromObjFile("meshes/Robot-parts-2018/head_s.obj"))),
          limb(std::make_shared<Mesh>(Mesh::fromObjFile("meshes/Robot-parts-2018/limb_s.obj"))),
          body(std::make_shared<Mesh>(Mesh::fromObjFile("meshes/Robot-parts-2018/body_s.obj"))) {

    root->addChild(body);
    root->addChild(std::make_shared<Wireframe>(Wireframe::fromAABB(boundingBox())));

    initHead();
    initArms();
    initLegs();
}

void Robot::initHead() {
    auto antenna_m = std::make_shared<Transform>(
            glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::vec3(0.3f)));
    antenna_m->addChild(antenna);
    auto antenna_l_m = std::make_shared<Transform>(
            glm::rotate(glm::pi<float>() / 4, glm::vec3(0.0f, 0.0f, 1.0f)));
    antenna_l_m->addChild(antenna_m);
    auto antenna_r_m = std::make_shared<Transform>(
            glm::rotate(-glm::pi<float>() / 4, glm::vec3(0.0f, 0.0f, 1.0f)));
    antenna_r_m->addChild(antenna_m);

    auto eyeball_m = std::make_shared<Transform>(
            glm::eulerAngleX(glm::pi<float>() * 0.4f) * glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)));
    eyeball_m->addChild(eyeball);
    auto eyeball_l_m = std::make_shared<Transform>(
            glm::eulerAngleY(glm::pi<float>() * 0.15f));
    eyeball_l_m->addChild(eyeball_m);
    auto eyeball_r_m = std::make_shared<Transform>(
            glm::eulerAngleY(glm::pi<float>() * -0.15f));
    eyeball_r_m->addChild(eyeball_m);

    headControl = std::make_shared<Transform>();
    headControl->addChild(head);
    headControl->addChild(antenna_l_m);
    headControl->addChild(antenna_r_m);
    headControl->addChild(eyeball_l_m);
    headControl->addChild(eyeball_r_m);

    auto head_m = std::make_shared<Transform>(glm::translate(glm::vec3(0.0f, 1.2f, 0.0f)));
    head_m->addChild(headControl);
    root->addChild(head_m);
}

void Robot::initArms() {
    auto arm = limb;
    auto arm_m = std::make_shared<Transform>(
            glm::translate(glm::vec3(0.0f, -0.8f, 0.0f)) * glm::scale(glm::vec3(1.0f, 2.0f, 1.0f)));
    arm_m->addChild(arm);

    leftArmControl = std::make_shared<Transform>();
    leftArmControl->addChild(arm_m);
    auto arm_l_b = std::make_shared<Transform>(glm::translate(glm::vec3(-1.4f, 0.6f, 0.0f)));
    arm_l_b->addChild(leftArmControl);

    rightArmControl = std::make_shared<Transform>();
    rightArmControl->addChild(arm_m);
    auto arm_r_b = std::make_shared<Transform>(glm::translate(glm::vec3(1.4f, 0.6f, 0.0f)));
    arm_r_b->addChild(rightArmControl);

    root->addChild(arm_l_b);
    root->addChild(arm_r_b);
}

void Robot::initLegs() {
    auto leg = limb;
    auto leg_m = std::make_shared<Transform>(glm::translate(glm::vec3(0.0f, -0.7f, 0.0f)));
    leg_m->addChild(leg);

    leftLegControl = std::make_shared<Transform>();
    leftLegControl->addChild(leg_m);
    auto leg_l_m = std::make_shared<Transform>(glm::translate(glm::vec3(-0.5f, -1.0f, 0.0f)));
    leg_l_m->addChild(leftLegControl);

    rightLegControl = std::make_shared<Transform>();
    rightLegControl->addChild(leg_m);
    auto leg_r_m = std::make_shared<Transform>(glm::translate(glm::vec3(0.5f, -1.0f, 0.0f)));
    leg_r_m->addChild(rightLegControl);

    root->addChild(leg_l_m);
    root->addChild(leg_r_m);
}

void Robot::draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) {
    root->draw(world, projection, view, eye);
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
