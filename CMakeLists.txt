cmake_minimum_required(VERSION 3.15)
project(Project1)

set(CMAKE_CXX_STANDARD 14)

find_package(GLEW REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)

aux_source_directory(. EXE_SRCS)

add_executable(Project1 "${EXE_SRCS}")
target_link_libraries(Project1 GLEW::glew_s GLFW3::GLFW3 glm)