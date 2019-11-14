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
#include "components/FreeFlying.h"
#include "gl_wraps/Shader.h"
#include "gl_wraps/GLContext.h"
#include "components/geometries/BezierCurve.h"
#include "objects/ControlTriple.h"
#include "components/ConstraintAnimator.h"

class Window : public GLContext {
public:
    Node scene;

    std::array<Camera *, 2> cameras;

    std::unique_ptr<Skybox> skybox;

    std::array<std::shared_ptr<Shader>, 2> shaders;

    std::shared_ptr<BezierCurve> bezier;

    std::vector<ControlTriple> controls;
    int selected = 0;

    std::shared_ptr<FreeFlying> flyControl;

    std::shared_ptr<ConstraintAnimator> animation;

    static Window *retrieve(GLFWwindow *w) { return reinterpret_cast<Window *>(glfwGetWindowUserPointer(w)); }

    Window();

    void loop();

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
