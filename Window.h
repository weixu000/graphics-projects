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

enum class Mode {
    MODEL,
    LIGHT,
    MODEL_LIGHT
};

class Window {
public:
    int width, height;
    const char *windowTitle = "GLFW Starter Project";
    Object *models[3], *currentObj;

    glm::vec3 eye{0, 0, 20}, center{0, 0, 0}, up{0, 1, 0};
    glm::mat4 projection = glm::perspective(glm::radians(60.0f),
                                            float(width) / float(height), 1.0f, 1000.0f),
            view = glm::lookAt(Window::eye, Window::center, Window::up);

    Shader normalShader, phongShader, *curShader;

    PointLight *light;

    Trackball trackball;

    Mode mode = Mode::MODEL;

    GLFWwindow *window;

    static Window *retrieve(GLFWwindow *w) { return reinterpret_cast<Window *>(glfwGetWindowUserPointer(w)); }

    Window();

    virtual ~Window();

    void setupCallbacks();

    void initializeProgram();

    void initializeObjects();

    void createWindow(int width, int height);

    void resizeCallback(int width, int height);

    void idleCallback();

    void displayCallback();

    void keyCallback(int key, int scancode, int action, int mods);

    void mouseButtonCallback(int button, int action, int mods);

    void cursorPosCallback(double x, double y);

    void scrollCallback(double xoffset, double yoffset);
};

#endif
