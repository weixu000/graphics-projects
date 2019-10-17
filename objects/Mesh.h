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
#include "../Material.h"

class Mesh : public Object {
private:
    std::vector<glm::vec3> points, normals;
    std::vector<glm::uvec3> face;
    GLuint vao, vbo[2], ebo;
    Material mat;

    void loadOBJ(const std::string &objFilename);

public:
    Mesh(const std::string &objFilename, const Material &m);

    ~Mesh();

    void draw(Shader &s);

    void update();
};


#endif //MESH_H
