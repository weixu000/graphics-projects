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
        components/Component.h
        components/Transform.cpp components/Transform.h
        components/Trackball.cpp components/Trackball.h
        components/geometries/Geometry.h components/geometries/Geometry.cpp
        components/geometries/Mesh.cpp components/geometries/Mesh.h
        components/geometries/Robot.cpp components/geometries/Robot.h
        components/geometries/Wireframe.cpp components/geometries/Wireframe.h
        objects/Camera.cpp objects/Camera.h
        components/FreeFlying.cpp components/FreeFlying.h
        components/HumanoidAnimation.cpp components/HumanoidAnimation.h)
target_link_libraries(Project3 GLEW::glew_s GLFW3::GLFW3 glm)