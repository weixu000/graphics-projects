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
#include <glm/gtx/transform.hpp>

#include "objects/Transform.h"
#include "objects/Robot.h"
#include "objects/Trackball.h"
#include "shaders/Shader.h"
#include "GLContext.h"

class Window : public GLContext {
public:
    glm::vec3 eye{0, 0, 20}, center{0, 0, 0}, up{0, 1, 0};
    glm::mat4 projection = glm::perspective(glm::radians(60.0f),
                                            float(width) / float(height), 1.0f, 1000.0f),
            view = glm::lookAt(Window::eye, Window::center, Window::up);

    std::shared_ptr<Transform> scene;

    std::shared_ptr<Shader> normalShader, phongShader;

    std::shared_ptr<Trackball> trackball;

    std::shared_ptr<Robot> robot;

    static Window *retrieve(GLFWwindow *w) { return reinterpret_cast<Window *>(glfwGetWindowUserPointer(w)); }

    Window();

    void loop();

    virtual ~Window();

    void setupCallbacks();

    void initializeProgram();

    void initializeObjects();

    void resizeCallback(int width, int height);

    void update();

    void draw();

    void keyCallback(int key, int scancode, int action, int mods);

    void mouseButtonCallback(int button, int action, int mods);

    void cursorPosCallback(double x, double y);

    void scrollCallback(double xoffset, double yoffset);
};

#endif
