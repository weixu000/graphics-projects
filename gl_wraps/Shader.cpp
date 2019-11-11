#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

enum ShaderType {
    vertex = GL_VERTEX_SHADER,
    fragment = GL_FRAGMENT_SHADER,
    geometry = GL_GEOMETRY_SHADER
};

GLuint LoadSingleShader(const std::string &shaderFilePath, ShaderType type) {
    // Create a shader id.
    GLuint shaderID = 0;
    shaderID = glCreateShader(type);

    // Try to read shader codes from the shader file.
    std::string shaderCode;
    std::ifstream shaderStream(shaderFilePath, std::ios::in);
    if (shaderStream.is_open()) {
        std::string Line;
        while (getline(shaderStream, Line))
            shaderCode += "\n" + Line;
        shaderStream.close();
    } else {
        throw std::runtime_error("Impossible to open " + shaderFilePath);
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Shader.
    std::cerr << "Compiling shader: " << shaderFilePath << std::endl;
    char const *sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer, nullptr);
    glCompileShader(shaderID);

    // Check Shader.
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> shaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(shaderID, InfoLogLength, nullptr, shaderErrorMessage.data());
        std::string msg(shaderErrorMessage.begin(), shaderErrorMessage.end());
        throw std::runtime_error(msg);
    }

    return shaderID;
}

void Shader::compileProgram(const std::string &vertexFilePath, const std::string &fragmentFilePath,
                            const std::string &geometryFilePath) {
    // Create the vertex shader and fragment shader.
    auto vertexShaderID = LoadSingleShader(vertexFilePath, vertex);
    auto fragmentShaderID = LoadSingleShader(fragmentFilePath, fragment);
    auto geometryShaderID = geometryFilePath.empty() ? 0 : LoadSingleShader(geometryFilePath, geometry);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Link the program.
    printf("Linking program\n");
    glAttachShader(id, vertexShaderID);
    glAttachShader(id, fragmentShaderID);
    if (geometryShaderID) glAttachShader(id, geometryShaderID);
    glLinkProgram(id);

    // Check the program.
    glGetProgramiv(id, GL_LINK_STATUS, &Result);
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(id, InfoLogLength, nullptr, ProgramErrorMessage.data());
        std::string msg(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
        glDeleteProgram(id);
        throw std::runtime_error(msg);
    } else {
        printf("Successfully linked program!\n");
    }

    // Detach and delete the shaders as they are no longer needed.
    glDetachShader(id, vertexShaderID);
    glDetachShader(id, fragmentShaderID);
    if (geometryShaderID) glDetachShader(id, geometryShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    if (geometryShaderID) glDeleteShader(geometryShaderID);
}

Shader::Shader(const std::string &vertex_file_path, const std::string &fragment_file_path,
               const std::string &geometry_file_path) {
    compileProgram(vertex_file_path, fragment_file_path, geometry_file_path);

    GLint count;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);
    GLint max_len;
    glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_len);
    for (GLint i = 0; i < count; i++) {
        std::string name(max_len, '\0');
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveUniform(id, i, max_len, &length, &size, &type, name.data());
        name.resize(length);
        uniformLocations[name] = glGetUniformLocation(id, name.c_str());
    }
}

void Shader::setUniformMatrix4(const std::string &name, const glm::mat4 &m) {
    if (uniformLocations.count(name)) {
        glUniformMatrix4fv(uniformLocations.at(name), 1, GL_FALSE, glm::value_ptr(m));
    }
}

void Shader::setUniform3f(const std::string &name, const glm::vec3 &v) {
    if (uniformLocations.count(name)) {
        glUniform3fv(uniformLocations.at(name), 1, glm::value_ptr(v));
    }
}

void Shader::setUniform1f(const std::string &name, const float f) {
    if (uniformLocations.count(name)) {
        glUniform1f(uniformLocations.at(name), f);
    }
}

void Shader::setUniform1i(const std::string &name, const int i) {
    if (uniformLocations.count(name)) {
        glUniform1i(uniformLocations.at(name), i);
    }
}
