#include "PointLight.h"
#include "shaders/Shader.h"

void PointLight::setUniform(Shader &s) {
    s.setUniform3f("lightPos", position);
    s.setUniform3f("lightColor", color);
    s.setUniform3f("ambientColor", ambient);
    s.setUniform1f("attenuation", attenuation);
}
