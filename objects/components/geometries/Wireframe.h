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
#include "../../../shaders/Shader.h"


class Wireframe : public Geometry {
public:
    Wireframe();

    explicit Wireframe(const std::vector<glm::vec3> &vertices, const std::vector<GLuint> &indices);

    ~Wireframe() override;

    Wireframe(const Wireframe &) = delete;

    Wireframe(Wireframe &&);

    Wireframe &operator=(const Wireframe &) = delete;

    Wireframe &operator=(Wireframe &&);

    static Wireframe fromAABB(const AABB &bb);

    void
    draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) override;

    AABB boundingBox() const override { return bb; }

private:
    GLsizei count = 0;
    GLuint vao = 0, vbo = 0, ebo = 0;

    AABB bb;

    static std::unique_ptr<Shader> shader; // wireframe use its own shader
};


#endif //WIREFRAME_H
