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

#include "objects/Node.h"
#include "objects/Camera.h"
#include "objects/Skybox.h"
#include "components/geometries/Robot.h"
#include "components/FreeFlying.h"
#include "shaders/Shader.h"
#include "GLContext.h"

class Window : public GLContext {
public:
    Node scene;

    Camera *cam;

    std::unique_ptr<Skybox> skybox;

    std::shared_ptr<Shader> normalShader;

    std::shared_ptr<FreeFlying> camCtl;
    bool shouldCull = true;

    std::vector<Node *> robots; // get number of robots culled

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
