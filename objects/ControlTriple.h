#ifndef CONTROLTRIPLE_H
#define CONTROLTRIPLE_H

#include <memory>

#include "../components/geometries/Mesh.h"
#include "../gl_wraps/Shader.h"

class ControlTriple {
public:
    ControlTriple(glm::vec3 *left, glm::vec3 *middle, glm::vec3 *right);

    void set(int i, const glm::vec3 &val);

    const glm::vec3 &get(int i) { return *points[i]; }

    void draw(const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye, GLint stencil);

    void upload();

private:
    std::array<glm::vec3 *, 3> points;

    static std::shared_ptr<Shader> shader;

    static std::shared_ptr<Mesh> controlIndicator, approxIndicator;

    GLBuffer vbo;
    GLVertexArray vao;
};


#endif //CONTROLTRIPLE_H
