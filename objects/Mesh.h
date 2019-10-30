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
#include "../Material.h"

class Mesh : public Geometry {
public:
    Mesh() = default;

    Mesh(std::vector<glm::vec3> _attrs, std::vector<GLuint> _indices);

    ~Mesh() override;

    Mesh(const Mesh &) = delete;

    Mesh(Mesh &&);

    Mesh &operator=(const Mesh &) = delete;

    Mesh &operator=(Mesh &&);

    static Mesh fromObjFile(const std::string &objFilename);

    void draw(const glm::mat4 &world) override;

    const glm::vec3 &minBound() const { return minVal; }

    const glm::vec3 &maxBound() const { return maxVal; }

    glm::vec3 center() const { return _center; }

    float scale() const { return _scale; }

    glm::mat4 normalizeMat() const;

private:
    GLsizei count = 0;
    GLuint vao = 0, vbo = 0, ebo = 0;
    std::shared_ptr<Material> mat;

    glm::vec3 minVal, maxVal, _center;
    float _scale;

    void computeStatistics(const std::vector<glm::vec3> &attrs, const std::vector<GLuint> &indices);
};


#endif //MESH_H
