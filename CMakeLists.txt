cmake_minimum_required(VERSION 3.15)
project(Project4)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(GLEW REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)

add_executable(Project4
        main.cpp
        Window.cpp Window.h
        gl_wraps/Shader.cpp gl_wraps/Shader.h
        gl_wraps/GLContext.h gl_wraps/GLContext.cpp
        gl_wraps/GLObject.h
        gl_wraps/GLBuffer.h
        gl_wraps/GLVertexArray.h
        Material.cpp Material.h
        Time.cpp Time.h
        objects/Node.h objects/Node.cpp
        components/Component.h
        components/Transform.cpp components/Transform.h
        components/Trackball.cpp components/Trackball.h
        components/geometries/Geometry.h components/geometries/Geometry.cpp
        components/geometries/Mesh.cpp components/geometries/Mesh.h
        components/geometries/Robot.cpp components/geometries/Robot.h
        components/geometries/Wireframe.cpp components/geometries/Wireframe.h
        objects/Camera.cpp objects/Camera.h
        components/FreeFlying.cpp components/FreeFlying.h
        components/HumanoidAnimation.cpp components/HumanoidAnimation.h
        objects/Skybox.cpp objects/Skybox.h
        textures/stb_image.c textures/stb_image.h
        components/geometries/BezierCurve.cpp components/geometries/BezierCurve.h
        objects/ControlTriple.cpp objects/ControlTriple.h
        components/ConstraintAnimator.cpp components/ConstraintAnimator.h)
target_link_libraries(Project4 GLEW::glew_s GLFW3::GLFW3 glm)