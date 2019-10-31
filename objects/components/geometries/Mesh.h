#ifndef MESH_H
#define MESH_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else

#include <GL/glew.h>

#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>

#include "Geometry.h"
#include "../../../shaders/Shader.h"
#include "../../../Material.h"

class Mesh : public Geometry {
public:
    Mesh() = default;

    Mesh(const std::vector<glm::vec3> &attrs, const std::vector<GLuint> &indices);

    ~Mesh() override;

    Mesh(const Mesh &) = delete;

    Mesh(Mesh &&);

    Mesh &operator=(const Mesh &) = delete;

    Mesh &operator=(Mesh &&);

    static Mesh fromObjFile(const std::string &objFilename);

    static Mesh cube();

    void useShader(const std::shared_ptr<Shader> &s) { shader = s; }

    void
    draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) override;

    AABB boundingBox() const override { return AABB{_minVal, _maxVal}; }

    const glm::vec3 &minVal() const { return _minVal; }

    const glm::vec3 &maxVal() const { return _maxVal; }

    glm::vec3 center() const { return _center; }

    float scale() const { return _scale; }

    glm::mat4 normalizeMat() const;

private:
    GLsizei count = 0;
    GLuint vao = 0, vbo = 0, ebo = 0;
    std::shared_ptr<Material> mat;

    glm::vec3 _minVal, _maxVal, _center;
    float _scale;

    std::shared_ptr<Shader> shader;

    void computeStatistics(const std::vector<glm::vec3> &attrs, const std::vector<GLuint> &indices);
};


#endif //MESH_H
