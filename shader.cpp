#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

enum ShaderType {
    vertex, fragment
};

GLuint LoadSingleShader(const char *shaderFilePath, ShaderType type) {
    // Create a shader id.
    GLuint shaderID = 0;
    if (type == vertex)
        shaderID = glCreateShader(GL_VERTEX_SHADER);
    else if (type == fragment)
        shaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Try to read shader codes from the shader file.
    std::string shaderCode;
    std::ifstream shaderStream(shaderFilePath, std::ios::in);
    if (shaderStream.is_open()) {
        std::string Line = "";
        while (getline(shaderStream, Line))
            shaderCode += "\n" + Line;
        shaderStream.close();
    } else {
        std::cerr << "Impossible to open " << shaderFilePath << ". "
                  << "Check to make sure the file exists and you passed in the "
                  << "right filepath!"
                  << std::endl;
        return 0;
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
        std::cerr << msg << std::endl;
        return 0;
    } else {
        if (type == vertex)
            printf("Successfully compiled vertex shader!\n");
        else if (type == fragment)
            printf("Successfully compiled fragment shader!\n");
    }

    return shaderID;
}

GLuint LoadShaders(const char *vertexFilePath, const char *fragmentFilePath) {
    // Create the vertex shader and fragment shader.
    GLuint vertexShaderID = LoadSingleShader(vertexFilePath, vertex);
    GLuint fragmentShaderID = LoadSingleShader(fragmentFilePath, fragment);

    // Check both shaders.
    if (vertexShaderID == 0 || fragmentShaderID == 0) return 0;

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Link the program.
    printf("Linking program\n");
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program.
    glGetProgramiv(programID, GL_LINK_STATUS, &Result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(programID, InfoLogLength, NULL, ProgramErrorMessage.data());
        std::string msg(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
        std::cerr << msg << std::endl;
        glDeleteProgram(programID);
        return 0;
    } else {
        printf("Successfully linked program!\n");
    }

    // Detach and delete the shaders as they are no longer needed.
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}

Shader::Shader(const char *vertex_file_path, const char *fragment_file_path) {
    programId = LoadShaders(vertex_file_path, fragment_file_path);
    if (programId == 0) {
        throw std::runtime_error("Shader compilation failed!");
    }

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
