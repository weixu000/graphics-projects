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
    Mesh(const std::string &objFilename, const Material &m);

    ~Mesh() override;

    void draw(const glm::mat4 &world) override;

    const glm::vec3 &minBound() const { return minVal; }

    const glm::vec3 &maxBound() const { return maxVal; }

    glm::vec3 center() const { return _center; }

    float scale() const { return _scale; }

    glm::mat4 normalizeMat() const;

private:
    std::vector<glm::vec3> points, normals;
    std::vector<glm::uvec3> indices;
    GLuint vao, vbo[2], ebo;
    Material mat;

    glm::vec3 minVal, maxVal, _center;
    float _scale;

    void loadOBJ(const std::string &objFilename);

    void computeStatistics();
};


#endif //MESH_H
