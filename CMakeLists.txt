cmake_minimum_required(VERSION 3.15)
project(Project2)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(GLEW REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)

add_executable(Project2
        main.cpp
        shader.cpp shader.h
        Window.cpp Window.h
        Object.h
        Cube.cpp Cube.h
        PointCloud.cpp PointCloud.h
        Mesh.cpp Mesh.h)
target_link_libraries(Project2 GLEW::glew_s GLFW3::GLFW3 glm)