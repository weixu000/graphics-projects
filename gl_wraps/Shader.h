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
#include "GLObject.h"

inline GLuint createProgram() { return glCreateProgram(); };

inline void delProgram(GLuint id) { glDeleteProgram(id); };

class Shader : public GLObject<createProgram, delProgram> {
public:
    Shader() = default;

    Shader(const std::string &vertex_file_path, const std::string &fragment_file_path,
           const std::string &geometry_file_path = "");

    void use() { glUseProgram(id); }

    void setUniformMatrix4(const std::string &name, const glm::mat4 &m);

    void setUniform3f(const std::string &name, const glm::vec3 &v);

    void setUniform1f(const std::string &name, float f);

    void setUniform1i(const std::string &name, int i);

private:
    std::unordered_map<std::string, GLint> uniformLocations;

    void compileProgram(const std::string &vertexFilePath, const std::string &fragmentFilePath,
                        const std::string &geometryFilePath);
};

#endif
