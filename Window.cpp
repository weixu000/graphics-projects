#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Window.h"

Window::Window() {
    setupCallbacks();
    initializeProgram();
    initializeObjects();

    // Initial size will not fire callback
    // force to fire
    resizeCallback(width, height);
}

void Window::setupCallbacks() {
    // Set the key callback.
    glfwSetKeyCallback(window,
                       [](GLFWwindow *window, int key, int scancode, int action, int mods) {
                           retrieve(window)->keyCallback(key, scancode, action, mods);
                       });
    // Set the window resize callback.
    glfwSetWindowSizeCallback(window,
                              [](GLFWwindow *window, int width, int height) {
                                  retrieve(window)->resizeCallback(width, height);
                              });
    // Set the mouse button callback.
    glfwSetMouseButtonCallback(window,
                               [](GLFWwindow *window, int button, int action, int mods) {
                                   retrieve(window)->mouseButtonCallback(button, action, mods);
                               });
    // Set the cursor position callback.
    glfwSetCursorPosCallback(window,
                             [](GLFWwindow *window, double x, double y) {
                                 retrieve(window)->cursorPosCallback(x, y);
                             });

    // Set the scroll callback.
    glfwSetScrollCallback(window,
                          [](GLFWwindow *window, double xoffset, double yoffset) {
                              retrieve(window)->scrollCallback(xoffset, yoffset);
                          });
}

void Window::initializeProgram() {
    try {
        normalShader = std::make_shared<Shader>("shaders/normal_coloring.vert",
                                                "shaders/normal_coloring.frag");
        phongShader = std::make_shared<Shader>("shaders/phong.vert",
                                               "shaders/phong.frag");
    } catch (...) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        exit(EXIT_FAILURE);
    }

    curShader = normalShader;
}

void Window::initializeObjects() {
    robot = std::make_shared<Robot>();
    robot->useShader(normalShader);

    trackball = std::make_shared<Trackball>();
    trackball->addChild(robot);

    scene = std::make_shared<Transform>();
    scene->addChild(trackball);
}

Window::~Window() {}

void Window::resizeCallback(int width, int height) {
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    if (width && height) {
        this->width = width;
        this->height = height;
        // Set the viewport size.
        glViewport(0, 0, width, height);

        // Set the projection matrix.
        projection = glm::perspective(glm::radians(60.0f),
                                      float(width) / float(height), 1.0f, 1000.0f);
    }
}

void Window::idleCallback() {
    // Perform any updates as necessary.
}

void Window::displayCallback() {
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Specify the values of the uniform variables we are going to use.
    curShader->use();
    curShader->setUniformMatrix4("projection", projection);
    curShader->setUniformMatrix4("view", view);
    curShader->setUniform3f("viewPos", eye);

    // Render the object.
    scene->draw(glm::mat4{1.0f});

    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

void Window::keyCallback(int key, int scancode, int action, int mods) {
    // Check for a key press.
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_N:
                curShader = (curShader == normalShader) ? phongShader : normalShader;
                break;
            default:
                break;
        }
    }
}

void Window::mouseButtonCallback(int button, int action, int mods) {
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            if (action == GLFW_PRESS) {
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                trackball->start(x / Window::width, y / Window::height);
            } else if (action == GLFW_RELEASE) {
                trackball->stop();
            }
            break;
        default:
            break;
    }
}

void Window::cursorPosCallback(double x, double y) {
    trackball->move(x / Window::width, y / Window::height);
}

void Window::scrollCallback(double xoffset, double yoffset) {
    trackball->scale(yoffset);
}
