#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <memory>
#include <glm/glm.hpp>

#include "objects/Mesh.h"

class Shader;

class PointLight {
public:
    glm::vec3 color;
    float attenuation;

    PointLight();

    void setUniform(Shader &s);

    void setPosition(const glm::vec3 &p);

    const glm::vec3 &position() const { return _position; }

    void draw(Shader &s);

private:
    glm::vec3 _position{0.0f};

    std::unique_ptr<Mesh> mesh;
};


#endif //POINTLIGHT_H
