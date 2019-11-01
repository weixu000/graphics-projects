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
        Time.cpp Time.h
        objects/Node.h objects/Node.cpp
        objects/components/Component.h
        objects/components/Transform.cpp objects/components/Transform.h
        objects/components/Trackball.cpp objects/components/Trackball.h
        objects/components/geometries/Geometry.h objects/components/geometries/Geometry.cpp
        objects/components/geometries/Mesh.cpp objects/components/geometries/Mesh.h
        objects/components/geometries/Robot.cpp objects/components/geometries/Robot.h
        objects/components/geometries/Wireframe.cpp objects/components/geometries/Wireframe.h
        objects/Camera.cpp objects/Camera.h
        objects/components/FreeFlying.cpp objects/components/FreeFlying.h
        objects/components/HumanoidAnimation.cpp objects/components/HumanoidAnimation.h)
target_link_libraries(Project3 GLEW::glew_s GLFW3::GLFW3 glm)