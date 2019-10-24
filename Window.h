#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else

#include <GL/glew.h>

#endif

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "objects/Object.h"
#include "shaders/Shader.h"
#include "PointLight.h"
#include "Trackball.h"

// Make sure OpenGL context is ready before anything else, since it is base class
class OpenGLContext {
public:
    GLFWwindow *window;

    int width = 640, height = 480;
    const char *windowTitle = "GLFW Starter Project";

    OpenGLContext();

    virtual ~OpenGLContext();

    OpenGLContext(const OpenGLContext &) = delete;

    OpenGLContext(OpenGLContext &&) = delete;

    OpenGLContext &operator=(const OpenGLContext &) = delete;

    OpenGLContext &operator=(OpenGLContext &&) = delete;
};

class Window : public OpenGLContext {
public:
    glm::vec3 eye{0, 0, 20}, center{0, 0, 0}, up{0, 1, 0};
    glm::mat4 projection = glm::perspective(glm::radians(60.0f),
                                            float(width) / float(height), 1.0f, 1000.0f),
            view = glm::lookAt(Window::eye, Window::center, Window::up);

    Object *models[3], *currentObj;

    Shader normalShader, phongShader, *curShader;

    PointLight *light;

    Trackball trackball;

    enum class Mode {
        MODEL,
        LIGHT,
        MODEL_LIGHT
    };

    Mode mode = Mode::MODEL;

    static Window *retrieve(GLFWwindow *w) { return reinterpret_cast<Window *>(glfwGetWindowUserPointer(w)); }

    Window();

    virtual ~Window();

    void setupCallbacks();

    void initializeProgram();

    void initializeObjects();

    void resizeCallback(int width, int height);

    void idleCallback();

    void displayCallback();

    void keyCallback(int key, int scancode, int action, int mods);

    void mouseButtonCallback(int button, int action, int mods);

    void cursorPosCallback(double x, double y);

    void scrollCallback(double xoffset, double yoffset);
};

#endif
