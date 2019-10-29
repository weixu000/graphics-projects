#include <glm/gtx/euler_angles.hpp>

#include "Robot.h"

Robot::Robot()
        : root(std::make_shared<Transform>()),
          antenna(std::make_shared<Mesh>("meshes/Robot-parts-2018/antenna_s.obj")),
          eyeball(std::make_shared<Mesh>("meshes/Robot-parts-2018/eyeball_s.obj")),
          head(std::make_shared<Mesh>("meshes/Robot-parts-2018/head_s.obj")),
          limb(std::make_shared<Mesh>("meshes/Robot-parts-2018/limb_s.obj")),
          body(std::make_shared<Mesh>("meshes/Robot-parts-2018/body_s.obj")) {

    root->addChild(body);

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

    auto head_m = std::make_shared<Transform>(glm::translate(glm::vec3(0.0f, 1.2f, 0.0f)));
    head_m->addChild(head);
    head_m->addChild(antenna_l_m);
    head_m->addChild(antenna_r_m);
    head_m->addChild(eyeball_l_m);
    head_m->addChild(eyeball_r_m);

    root->addChild(head_m);
}

void Robot::initArms() {
    auto arm = limb;
    auto arm_m = std::make_shared<Transform>(
            glm::eulerAngleZ(glm::pi<float>() / 2) * glm::scale(glm::vec3(1.0f, 2.0f, 1.0f)));
    arm_m->addChild(arm);
    auto arm_l_m = std::make_shared<Transform>(glm::translate(glm::vec3(-2.2f, 0.8f, 0.0f)));
    arm_l_m->addChild(arm_m);
    auto arm_r_m = std::make_shared<Transform>(glm::translate(glm::vec3(2.2f, 0.8f, 0.0f)));
    arm_r_m->addChild(arm_m);

    root->addChild(arm_l_m);
    root->addChild(arm_r_m);
}

void Robot::initLegs() {
    auto leg = limb;
    leg->useShader(shader);
    auto leg_l_m = std::make_shared<Transform>(glm::translate(glm::vec3(-0.5f, -0.7f, 0.0f)));
    leg_l_m->addChild(leg);
    auto leg_r_m = std::make_shared<Transform>(glm::translate(glm::vec3(0.5f, -0.7f, 0.0f)));
    leg_r_m->addChild(leg);
    auto leg_base_m = std::make_shared<Transform>(glm::translate(glm::vec3(0.0f, -1.0f, 0.0f)));
    leg_base_m->addChild(leg_l_m);
    leg_base_m->addChild(leg_r_m);
    root->addChild(leg_base_m);
}

void Robot::draw(const glm::mat4 &world) {
    root->draw(world);
}

void Robot::useShader(const std::shared_ptr<Shader> &s) {
    Geometry::useShader(s);
    antenna->useShader(shader);
    eyeball->useShader(shader);
    head->useShader(shader);
    limb->useShader(shader);
    body->useShader(shader);
}
