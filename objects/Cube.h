#ifndef _CUBE_H_
#define _CUBE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else

#include <GL/glew.h>

#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Geometry.h"

class Cube : public Geometry {
private:
    GLuint vao;
    GLuint vbo;
public:
    Cube();

    ~Cube() override;

    void draw(const glm::mat4 &world) override;
};

#endif

