#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Window.h"
#include "Time.h"
#include "objects/components/HumanoidAnimation.h"

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
}

void Window::initializeObjects() {
    auto animation = std::make_shared<HumanoidAnimation>();
    scene.addComponent(animation);
    auto robot = std::make_shared<Robot>(animation->headControl, animation->leftArmControl, animation->rightArmControl,
                                         animation->leftLegControl, animation->rightLegControl);
    robot->useShader(normalShader);

    for (auto i = -5; i < 5; ++i) {
        for (auto j = -5; j < 5; ++j) {
            auto grid = std::make_unique<Node>(
                    glm::translate(glm::vec3(i * 2, 0, j * 2)) * glm::scale(glm::vec3(0.2f, 0.2f, 0.2f)));
            grid->addComponent(robot);

            robots.push_back(scene.addChild(std::move(grid)));
        }
    }

    glm::vec3 eye(0, 0, 20);
    glm::mat4 projection = glm::perspective(glm::radians(60.0f),
                                            float(width) / float(height), 1.0f, 1000.0f);

    camCtl = std::make_shared<FreeFlying>(glm::translate(eye));
    cam = static_cast<Camera *>(scene.addChild(std::make_unique<Camera>(projection, camCtl)));
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
        cam->projection = glm::perspective(glm::radians(60.0f),
                                           float(width) / float(height), 1.0f, 1000.0f);
    }
}

void Window::update() {
    scene.update();
}

void Window::draw() {
    cam->setup();

    if (shouldCull) {
        scene.cull(cam->projection * cam->view);
        size_t num = 0;
        for (auto n:robots) {
            if (!n->culled()) {
                ++num;
            }
        }
        glfwSetWindowTitle(window, ("Not culled: " + std::to_string(num)).c_str());
    }

    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the object.
    scene.draw(glm::mat4(1.0f), cam->projection, cam->view, cam->eye);

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
            case GLFW_KEY_W:
                camCtl->forward = true;
                break;
            case GLFW_KEY_A:
                camCtl->left = true;
                break;
            case GLFW_KEY_S:
                camCtl->backward = true;
                break;
            case GLFW_KEY_D:
                camCtl->right = true;
                break;
            case GLFW_KEY_Q:
                camCtl->up = true;
                break;
            case GLFW_KEY_E:
                camCtl->down = true;
                break;
            case GLFW_KEY_LEFT_SHIFT:
                camCtl->local = false;
                break;
            case GLFW_KEY_C:
                shouldCull = !shouldCull;
                std::cout << "shouldCull: " << shouldCull << std::endl;
                break;
            default:
                break;
        }
    } else if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_W:
                camCtl->forward = false;
                break;
            case GLFW_KEY_A:
                camCtl->left = false;
                break;
            case GLFW_KEY_S:
                camCtl->backward = false;
                break;
            case GLFW_KEY_D:
                camCtl->right = false;
                break;
            case GLFW_KEY_Q:
                camCtl->up = false;
                break;
            case GLFW_KEY_E:
                camCtl->down = false;
                break;
            case GLFW_KEY_LEFT_SHIFT:
                camCtl->local = true;
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
                camCtl->startRotate(x / Window::width, y / Window::height);
            } else if (action == GLFW_RELEASE) {
                camCtl->stopRotate();
            }
            break;
        default:
            break;
    }
}

void Window::cursorPosCallback(double x, double y) {
    camCtl->rotate(x / Window::width, y / Window::height);
}

void Window::scrollCallback(double xoffset, double yoffset) {
}

void Window::loop() {
    Time::reset();
    while (!glfwWindowShouldClose(window)) {
        Time::tick();
        glfwPollEvents();
        update();
        draw();
    }
}
