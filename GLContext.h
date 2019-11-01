#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else

#include <GL/glew.h>

#endif

#include <GLFW/glfw3.h>

// Make sure OpenGL context is ready before anything else, since it is base class
class GLContext {
public:
    GLFWwindow *window;

    int width = 640, height = 480;

    GLContext();

    virtual ~GLContext();

    GLContext(const GLContext &) = delete;

    GLContext(GLContext &&) = delete;

    GLContext &operator=(const GLContext &) = delete;

    GLContext &operator=(GLContext &&) = delete;
};

#endif //OPENGLCONTEXT_H
