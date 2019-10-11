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

#include "Object.h"

class Mesh : public Object {
private:
    std::vector<glm::vec3> points, normals;
    std::vector<glm::uvec3> face;
    GLuint vao, vbo, ebo;
public:
    explicit Mesh(const std::string &objFilename);

    ~Mesh();

    void draw();

    void update();

    void loadOBJ(const std::string &objFilename);
};


#endif //MESH_H
