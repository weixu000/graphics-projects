#include "Material.h"
#include "shaders/Shader.h"

void Material::setUniform(Shader &s) {
    s.setUniform3f("ks", ks);
    s.setUniform3f("kd", kd);
    s.setUniform3f("ka", ka);
    s.setUniform1f("alpha", alpha);
}
