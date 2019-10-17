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
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "objects/Mesh.h"
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
    static int width;
    static int height;
    static const char *windowTitle;
    static Mesh *models[3];
    static Mesh *currentObj;
    static glm::mat4 projection;
    static glm::mat4 view;
    static glm::vec3 eye, center, up;

    static Shader normalShader, phongShader, *curShader;

    static PointLight light;

    static Trackball trackball;

    static Mode mode;

    static bool initializeProgram();

    static bool initializeObjects();

    static void cleanUp();

    static GLFWwindow *createWindow(int width, int height);

    static void resizeCallback(GLFWwindow *window, int width, int height);

    static void idleCallback();

    static void displayCallback(GLFWwindow *);

    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

    static void cursorPosCallback(GLFWwindow *window, double x, double y);

    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
};

#endif
