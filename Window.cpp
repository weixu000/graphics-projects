#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "Window.h"

OpenGLContext::OpenGLContext() {
    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create the GLFW window.
    window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(window, this);

    // Make the context of the window.
    glfwMakeContextCurrent(window);
#ifndef __APPLE__
    // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.
    // Initialize GLEW.
    if (glewInit()) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
#endif
    // Get info of GPU and supported OpenGL version.
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)
              << std::endl;

    //If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
    std::cout << "Supported GLSL version is: " <<
              glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif

    // Set swap interval to 1.
    glfwSwapInterval(0);
}

OpenGLContext::~OpenGLContext() {
    glfwDestroyWindow(window);
}

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
        normalShader = Shader("shaders/normal_coloring.vert", "shaders/normal_coloring.frag");
        phongShader = Shader("shaders/phong.vert", "shaders/phong.frag");
    } catch (...) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        exit(EXIT_FAILURE);
    }

    curShader = &phongShader;
}

void Window::initializeObjects() {
    Material mat;

    //shiny
    mat.ks = glm::vec3(0.727811f, 0.626959f, 0.626959f);
    mat.kd = glm::vec3(0.0f);
    mat.ka = glm::vec3(0.1745f, 0.01175f, 0.01175f);
    mat.alpha = 0.6f * 128;
    models[0] = new Mesh("meshes/bunny.obj", mat);

    // diffuse
    mat.ks = glm::vec3(0.0f);
    mat.kd = glm::vec3(0.7038f, 0.27048f, 0.0828f);
    mat.ka = glm::vec3(0.19125f, 0.0735f, 0.0225);
    mat.alpha = 1.0f;
    models[1] = new Mesh("meshes/dragon.obj", mat);

    // diffuse and shiny
    mat.ks = glm::vec3(0.633f, 0.727811f, 0.633f);
    mat.kd = glm::vec3(0.07568f, 0.61424f, 0.07568f);
    mat.ka = glm::vec3(0.0215f, 0.1745f, 0.0215f);
    mat.alpha = 0.6f * 128;
    models[2] = new Mesh("meshes/bear.obj", mat);
//    models[1] = models[2] = models[0];

    // Set cube to be the first to display
    currentObj = models[0];

    light = new PointLight;
    light->color = glm::vec3(0.3f, 0.6f, 0.6f) * 30.0f;
    light->setPosition(glm::vec3(10.0f));
    light->attenuation = 1.0f;
}

Window::~Window() {
    // Deallcoate the objects.
    for (auto m:models) {
        delete m;
    }

    delete light;
}

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
    currentObj->update();
}

void Window::displayCallback() {
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Specify the values of the uniform variables we are going to use.
    curShader->use();
    curShader->setUniformMatrix4("projection", projection);
    curShader->setUniformMatrix4("view", view);
    curShader->setUniform3f("viewPos", eye);

    light->setUniform(*curShader);
    light->draw(*curShader);

    // Render the object.
    currentObj->draw(*curShader);

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
            case GLFW_KEY_F1:
                currentObj = models[0];
                break;
            case GLFW_KEY_F2:
                currentObj = models[1];
                break;
            case GLFW_KEY_F3:
                currentObj = models[2];
                break;
            case GLFW_KEY_N:
                curShader = (curShader == &normalShader) ? &phongShader : &normalShader;
                break;
            case GLFW_KEY_1:
                mode = Mode::MODEL;
                break;
            case GLFW_KEY_2:
                mode = Mode::LIGHT;
                break;
            case GLFW_KEY_3:
                mode = Mode::MODEL_LIGHT;
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
                trackball.start(x / Window::width, y / Window::height);
            } else if (action == GLFW_RELEASE) {
                trackball.stop();
            }
            break;
        default:
            break;
    }
}

void Window::cursorPosCallback(double x, double y) {
    auto delta = trackball.move(x / Window::width, y / Window::height);
    switch (mode) {
        case Mode::MODEL:
            currentObj->getModel() = delta * currentObj->getModel();
            break;
        case Mode::LIGHT:
            light->setPosition(glm::mat3(delta) * light->position());
            break;
        case Mode::MODEL_LIGHT:
            currentObj->getModel() = delta * currentObj->getModel();
            light->setPosition(glm::mat3(delta) * light->position());
            break;
    }
}

void Window::scrollCallback(double xoffset, double yoffset) {
    auto delta = Trackball::scale(yoffset);
    switch (mode) {
        case Mode::LIGHT:
            light->setPosition(glm::mat3(delta) * light->position());
            break;
        case Mode::MODEL:
            currentObj->getModel() = delta * currentObj->getModel();
            break;
        case Mode::MODEL_LIGHT:
            light->setPosition(glm::mat3(delta) * light->position());
            currentObj->getModel() = delta * currentObj->getModel();
            break;
    }
}
