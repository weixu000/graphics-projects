#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

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

class PointCloud : public Object {
private:
    std::vector<glm::vec3> points;
    GLuint vao, vbo;
    GLfloat pointSize;
public:
    PointCloud(std::string objFilename, GLfloat pointSize = 10);

    ~PointCloud();

    void draw();

    void update();

    void updatePointSize(GLfloat delta);

    void spin(float deg);
};

#endif

