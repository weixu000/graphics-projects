#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

class Shader;

class Material {
public:
    glm::vec3 ks, kd, ka;
    float alpha;

    void setUniform(Shader &s);
};


#endif //MATERIAL_H
