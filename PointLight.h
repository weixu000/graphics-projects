#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <glm/glm.hpp>

class Shader;

class PointLight {
public:
    glm::vec3 position;
    glm::vec3 color, ambient;
    float attenuation;

    void setUniform(Shader &s);
};


#endif //POINTLIGHT_H
