#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else

#include <GL/glew.h>

#endif

#include <GLFW/glfw3.h>
#include <cstdlib>

#include "Window.h"


void error_callback(int error, const char *description) {
    // Print error.
    std::cerr << description << std::endl;
    exit(EXIT_FAILURE);
}

void setup_opengl_settings() {
    // Enable depth buffering.
    glEnable(GL_DEPTH_TEST);
    // Related to shaders and z value comparisons for the depth buffer.
    glDepthFunc(GL_LEQUAL);
    // Set polygon drawing mode to fill front and back of each polygon.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Set clear color to black.
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void setup_glfw() {
    // Initialize GLFW.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

#ifdef __APPLE__
    // Apple implements its own version of OpenGL and requires special treatments
    // to make it uses modern OpenGL.

    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Setup error callback.
    glfwSetErrorCallback(error_callback);
}

int main(void) {
    setup_glfw();

    Window window;
    // Setup OpenGL settings.
    setup_opengl_settings();
    // Loop while GLFW window should stay open.
    while (!glfwWindowShouldClose(window.window)) {
        // Main render display callback. Rendering of objects is done here.
        window.displayCallback();
        // Idle callback. Updating objects, etc. can be done here.
        window.idleCallback();
    }
}
