#include <iostream>

#include "GLContext.h"

GLContext::GLContext() {
    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create the GLFW window.
    window = glfwCreateWindow(width, height, "GLFW Starter Project", NULL, NULL);
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

GLContext::~GLContext() {
    glfwDestroyWindow(window);
}