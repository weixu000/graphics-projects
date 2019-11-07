#ifndef SHADER_HPP
#define SHADER_HPP

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else

#include <GL/glew.h>

#endif

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map>

#include <glm/glm.hpp>

class Shader {
public:
    Shader() = default;

    Shader(const char *vertex_file_path, const char *fragment_file_path);

    Shader(const Shader &) = delete;

    Shader(Shader &&s) noexcept;

    Shader &operator=(const Shader &) = delete;

    Shader &operator=(Shader &&s) noexcept;


    ~Shader() { glDeleteProgram(programId); }

    void use() { glUseProgram(programId); }

    void setUniformMatrix4(const std::string &name, const glm::mat4 &m);

    void setUniform3f(const std::string &name, const glm::vec3 &v);

    void setUniform1f(const std::string &name, const float f);

    void setUniform1i(const std::string &name, const int i);


private:
    GLuint programId = 0U;

    std::unordered_map<std::string, GLint> uniformLocations;
};

#endif
