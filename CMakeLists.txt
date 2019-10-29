cmake_minimum_required(VERSION 3.15)
project(Project3)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(GLEW REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)

add_executable(Project3
        main.cpp
        shaders/Shader.cpp shaders/Shader.h
        Window.cpp Window.h
        GLContext.h GLContext.cpp
        Material.cpp Material.h
        PointLight.cpp PointLight.h
        objects/Node.h
        objects/Geometry.h
        objects/Cube.cpp objects/Cube.h
        objects/Mesh.cpp objects/Mesh.h
        objects/Transform.cpp objects/Transform.h
        objects/Trackball.cpp objects/Trackball.h
        objects/Robot.cpp objects/Robot.h)
target_link_libraries(Project3 GLEW::glew_s GLFW3::GLFW3 glm)