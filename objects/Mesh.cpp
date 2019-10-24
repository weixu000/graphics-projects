#include <fstream>
#include <sstream>
#include <iostream>

#include "Mesh.h"
#include "../shaders/Shader.h"

Mesh::Mesh(const std::string &objFilename, const Material &m)
        : mat(m) {
    loadOBJ(objFilename);

    computeStatistics();

    // Generate VAO, VBO, EBO.
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbo);
    glGenBuffers(1, &ebo);

    // Bind to the VAO.
    glBindVertexArray(vao);

    // Bind to the first VBO.
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
                 points.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO.
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
                 normals.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 1.
    // We will be able to access points through it.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind to the EBO.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // Pass in the data.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3) * indices.size(),
                 indices.data(), GL_STATIC_DRAW);

    // Unbind from the VAO.
    glBindVertexArray(0);
}

void Mesh::loadOBJ(const std::string &objFilename) {
    std::ifstream objFile(objFilename);
    if (objFile.is_open()) {
        std::string line;
        while (std::getline(objFile, line)) {
            std::istringstream ss(line);
            std::string label;
            ss >> label;
            if (label == "v") {
                glm::vec3 p;
                ss >> p.x >> p.y >> p.z;
                points.push_back(p);
            } else if (label == "vn") {
                glm::vec3 n;
                ss >> n.x >> n.y >> n.z;
                normals.push_back(glm::normalize(n));
            } else if (label == "f") {
                glm::uvec3 f;
                char delim;
                ss >> f.x >> delim >> delim >> f.x
                   >> f.y >> delim >> delim >> f.y
                   >> f.z >> delim >> delim >> f.z;
                indices.push_back(f - 1U);
            }
        }
        std::cout << objFilename << " " << points.size() << std::endl;
    } else {
        std::cerr << "Can't open the file " << objFilename << std::endl;
    }
    objFile.close();
}

Mesh::~Mesh() {
    // Delete VBO, EBO, VAO.
    glDeleteBuffers(2, vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void Mesh::draw(const glm::mat4 &world) {
    mat.setUniform(*shader);
    shader->setUniformMatrix4("model", world);
    // Bind to the VAO.
    glBindVertexArray(vao);
    // Draw points
    glDrawElements(GL_TRIANGLES, 3 * indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

glm::mat4 Mesh::normalizeMat() const {
    glm::mat4 model{1.0f};
    model = glm::scale(model, glm::vec3(11.5 / _scale));
    model = glm::translate(model, -_center);
    return model;
}

void Mesh::computeStatistics() {
    minVal = points[0], maxVal = points[0];
    for (auto &p:points) {
        minVal = glm::min(minVal, p);
        maxVal = glm::max(maxVal, p);
    }
    _center = (maxVal + minVal) / 2.0f;

    _scale = 0.0f;
    for (auto &p:points) {
        _scale = glm::max(glm::length(p - _center), _scale);
    }
}
