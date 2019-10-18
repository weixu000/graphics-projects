#include "PointLight.h"
#include "shaders/Shader.h"

PointLight::PointLight() {
    Material mat;
    mat.ks = glm::vec3(0.0f);
    mat.kd = glm::vec3(0.0f);
    mat.ka = glm::vec3(1.0f);
    mat.alpha = 1.0f;

    mesh = std::make_unique<Mesh>("meshes/sphere.obj", mat);
    mesh->getModel() = glm::scale(glm::vec3(1 / 100.0f)) * mesh->getModel();
}

void PointLight::setPosition(const glm::vec3 &p) {
    mesh->getModel() = glm::translate(p) * glm::translate(-_position) * mesh->getModel();
    _position = p;
}

void PointLight::setUniform(Shader &s) {
    s.setUniform3f("lightPos", _position);
    s.setUniform3f("lightColor", color);
    s.setUniform1f("attenuation", attenuation);
}

void PointLight::draw(Shader &s) {
    mesh->draw(s);
}
