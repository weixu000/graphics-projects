#include "Window.h"

int Window::width;
int Window::height;

const char *Window::windowTitle = "GLFW Starter Project";

// Objects to display.
Mesh *Window::models[3];

// The object currently displaying.
Mesh *Window::currentObj;

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::eye(0, 0, 20); // Camera position.
glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

Shader Window::normalShader, Window::phongShader, *Window::curShader;

PointLight Window::light;

Trackball Window::trackball;


bool Window::initializeProgram() {
    try {
        normalShader = Shader("shaders/normal_coloring.vert", "shaders/normal_coloring.frag");
        phongShader = Shader("shaders/phong.vert", "shaders/phong.frag");
    } catch (...) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    curShader = &phongShader;

    return true;
}

bool Window::initializeObjects() {
    Material mat;

    //shiny
    mat.ks = glm::vec3(1.0f);
    mat.kd = glm::vec3(0.0f);
    mat.ka = glm::vec3(0.01f);
    mat.alpha = 10.0f;
    models[0] = new Mesh("meshes/bunny.obj", mat);

    // diffuse
    mat.ks = glm::vec3(0.0f);
    mat.kd = glm::vec3(1.0f);
    mat.ka = glm::vec3(0.01f);
    mat.alpha = 10.0f;
    models[1] = new Mesh("meshes/dragon.obj", mat);

    // diffuse and shiny
    mat.ks = glm::vec3(1.0f);
    mat.kd = glm::vec3(1.0f);
    mat.ka = glm::vec3(0.01f);
    mat.alpha = 10.0f;
    models[2] = new Mesh("meshes/bear.obj", mat);
//    models[1] = models[2] = models[0];

    // Set cube to be the first to display
    currentObj = models[0];

    light.color = glm::vec3(20.0f);
    light.ambient = glm::vec3(1.0f);
    light.position = glm::vec3(15.0f);
    light.attenuation = 0.5f;

    return true;
}

void Window::cleanUp() {
    // Deallcoate the objects.
    for (auto m:models) {
        delete m;
    }
}

GLFWwindow *Window::createWindow(int width, int height) {
    // Initialize GLFW.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

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

    // Create the GLFW window.
    GLFWwindow *window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

#ifndef __APPLE__
    // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

    // Initialize GLEW.
    if (glewInit()) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return NULL;
    }
#endif

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);

    return window;
}

void Window::resizeCallback(GLFWwindow *window, int width, int height) {
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    // Set the projection matrix.
    Window::projection = glm::perspective(glm::radians(60.0),
                                          double(width) / (double) height, 1.0, 1000.0);
}

void Window::idleCallback() {
    // Perform any updates as necessary.
    currentObj->update();
}

void Window::displayCallback(GLFWwindow *window) {
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Specify the values of the uniform variables we are going to use.
    curShader->use();
    curShader->setUniformMatrix4("projection", projection);
    curShader->setUniformMatrix4("view", view);
    curShader->setUniformMatrix4("model", currentObj->getModel());
    curShader->setUniform3f("viewPos", eye);

    light.setUniform(*curShader);

    // Render the object.
    currentObj->draw(*curShader);

    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
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
            default:
                break;
        }
    }
}

void Window::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            if (action == GLFW_PRESS) {
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                trackball.start(x, y);
            } else if (action == GLFW_RELEASE) {
                trackball.stop();
            }
            break;
        default:
            break;
    }
}

void Window::cursorPosCallback(GLFWwindow *window, double x, double y) {
    currentObj->getModel() = trackball.move(x, y) * currentObj->getModel();
}

void Window::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    currentObj->getModel() = trackball.scale(yoffset) * currentObj->getModel();
}
