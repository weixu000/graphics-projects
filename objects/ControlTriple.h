#ifndef CONTROLTRIPLE_H
#define CONTROLTRIPLE_H

#include <memory>

#include "../components/geometries/Mesh.h"
#include "../gl_wraps/Shader.h"

class ControlTriple {
public:
    ControlTriple(glm::vec3 *left, glm::vec3 *middle, glm::vec3 *right);

    void setLeft(const glm::vec3 &val);

    void setRight(const glm::vec3 &val);

    void setMiddle(const glm::vec3 &val);

    void draw(const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye);

    void upload();

private:
    glm::vec3 *left, *middle, *right;

    static std::shared_ptr<Shader> shader;

    static std::shared_ptr<Mesh> controlIndicator, approxIndicator;

    GLBuffer vbo;
    GLVertexArray vao;
};


#endif //CONTROLTRIPLE_H
