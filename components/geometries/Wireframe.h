#ifndef WIREFRAME_H
#define WIREFRAME_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else

#include <GL/glew.h>

#endif

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "Geometry.h"
#include "../../gl_wraps/Shader.h"
#include "../../gl_wraps/GLBuffer.h"
#include "../../gl_wraps/GLVertexArray.h"


class Wireframe : public Geometry {
public:
    Wireframe();

    explicit Wireframe(const std::vector<glm::vec3> &vertices, const std::vector<GLuint> &indices);

    static Wireframe fromAABB(const AABB &bb);

    void
    draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) override;

    AABB boundingBox() const override { return bb; }

private:
    GLsizei count = 0;
    GLVertexArray vao;
    GLBuffer vbo, ebo;

    AABB bb;

    static std::unique_ptr<Shader> shader; // wireframe use its own shader
};


#endif //WIREFRAME_H
