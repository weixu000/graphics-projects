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
    glShaderSource(shaderID, 1, &sourcePointer, NULL);
    glCompileShader(shaderID);

    // Check Shader.
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> shaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(shaderID, InfoLogLength, NULL, shaderErrorMessage.data());
        std::string msg(shaderErrorMessage.begin(), shaderErrorMessage.end());
        throw std::runtime_error(msg);
    }

    return shaderID;
}

GLuint LoadShaders(const std::string &vertexFilePath, const std::string &fragmentFilePath,
                   const std::string &geometryFilePath = "") {
    // Create the vertex shader and fragment shader.
    GLuint vertexShaderID = LoadSingleShader(vertexFilePath, vertex);
    GLuint fragmentShaderID = LoadSingleShader(fragmentFilePath, fragment);
    GLuint geometryShaderID = geometryFilePath.empty() ? 0 : LoadSingleShader(geometryFilePath, geometry);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Link the program.
    printf("Linking program\n");
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    if (geometryShaderID) glAttachShader(programID, geometryShaderID);
    glLinkProgram(programID);

    // Check the program.
    glGetProgramiv(programID, GL_LINK_STATUS, &Result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(programID, InfoLogLength, NULL, ProgramErrorMessage.data());
        std::string msg(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
        glDeleteProgram(programID);
        throw std::runtime_error(msg);
    } else {
        printf("Successfully linked program!\n");
    }

    // Detach and delete the shaders as they are no longer needed.
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    if (geometryShaderID) glDetachShader(programID, geometryShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    if (geometryShaderID) glDeleteShader(geometryShaderID);

    return programID;
}

Shader::Shader(const std::string &vertex_file_path, const std::string &fragment_file_path,
               const std::string &geometry_file_path) {
    programId = LoadShaders(vertex_file_path, fragment_file_path, geometry_file_path);

    GLint count;
    glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &count);
    GLint max_len;
    glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_len);
    for (GLint i = 0; i < count; i++) {
        std::string name(GL_ACTIVE_UNIFORM_MAX_LENGTH, '\0');
        GLsizei length;
        glGetActiveUniform(programId, i, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length, nullptr, nullptr, name.data());
        name.resize(length);
        uniformLocations[name] = glGetUniformLocation(programId, name.c_str());
    }
}

Shader::Shader(Shader &&s) noexcept {
    programId = s.programId;
    s.programId = 0U;

    uniformLocations = std::move(s.uniformLocations);
}

Shader &Shader::operator=(Shader &&s) noexcept {
    glDeleteProgram(programId);
    programId = s.programId;
    s.programId = 0U;

    uniformLocations = std::move(s.uniformLocations);

    return *this;
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
