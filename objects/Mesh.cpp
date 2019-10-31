#include <iostream>
#include <fstream>
#include <map>
#include <tuple>
#include <numeric>

#include "Mesh.h"
#include "../shaders/Shader.h"

Mesh::Mesh(const std::vector<glm::vec3> &attrs, const std::vector<GLuint> &indices) {
    count = indices.size();

    computeStatistics(attrs, indices);

    // Generate VAO, VBO, EBO.
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Bind to the VAO.
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * attrs.size(),
                 attrs.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          2 * sizeof(glm::vec3), nullptr);
    // Enable vertex attribute 1.
    // We will be able to access normals through it.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3),
                          reinterpret_cast<void *>(sizeof(glm::vec3)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // Pass in the data.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(),
                 indices.data(), GL_STATIC_DRAW);

    // Unbind from the VAO.
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    // Delete VBO, EBO, VAO.
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

Mesh::Mesh(Mesh &&other)
        : Mesh() {
    *this = std::move(other);
}


Mesh &Mesh::operator=(Mesh &&other) {
    count = other.count;
    vbo = other.vbo;
    vao = other.vao;
    ebo = other.ebo;
    mat = std::move(other.mat);
    _minVal = other._minVal;
    _maxVal = other._maxVal;
    _scale = other._scale;

    other.count = 0;
    other.vbo = other.vao = other.ebo = 0;

    return *this;
}

void Mesh::draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) {
    if (mat) {
        mat->setUniform(*shader);
    }
    assert(shader);
    shader->use();
    shader->setUniformMatrix4("projection", projection);
    shader->setUniformMatrix4("view", view);
    shader->setUniform3f("viewPos", eye);
    shader->setUniformMatrix4("model", world);
    // Bind to the VAO.
    glBindVertexArray(vao);
    // Draw points
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

glm::mat4 Mesh::normalizeMat() const {
    glm::mat4 model{1.0f};
    model = glm::scale(model, glm::vec3(11.5 / _scale));
    model = glm::translate(model, -_center);
    return model;
}

void Mesh::computeStatistics(const std::vector<glm::vec3> &attrs, const std::vector<GLuint> &indices) {
    _minVal = attrs[0], _maxVal = attrs[0];
    for (auto it = attrs.begin(); it != attrs.end(); it += 2) {
        _minVal = glm::min(_minVal, *it);
        _maxVal = glm::max(_maxVal, *it);
    }
    _center = (_maxVal + _minVal) / 2.0f;

    _scale = 0.0f;
    for (auto it = attrs.begin(); it != attrs.end(); it += 2) {
        _scale = glm::max(glm::length(*it - _center), _scale);
    }
}

Mesh Mesh::fromObjFile(const std::string &objFilename) {
    std::ifstream objFile(objFilename);
    if (!objFile.is_open()) {
        std::cerr << "Can't open the file " << objFilename << std::endl;
        return Mesh();
    }
    std::vector<glm::vec3> vertices, normals;
    std::vector<GLuint> faces;
    while (objFile) {
        std::string label;
        objFile >> label;
        if (label == "v") {
            glm::vec3 p;
            objFile >> p.x >> p.y >> p.z;
            vertices.push_back(p);
        } else if (label == "vn") {
            glm::vec3 n;
            objFile >> n.x >> n.y >> n.z;
            normals.push_back(glm::normalize(n));
        } else if (label == "f") {
            for (int i = 0; i < 3; ++i) {
                GLuint v, n;
                objFile >> v;
                faces.push_back(v - 1);
                objFile.ignore(std::numeric_limits<std::streamsize>::max(), '/');
                objFile.ignore(std::numeric_limits<std::streamsize>::max(), '/');
                objFile >> n;
                faces.push_back(n - 1);
            }
        }
        objFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    objFile.close();

    std::vector<glm::vec3> attrs; // vertices and normals interleaved
    std::vector<GLuint> indices;

    // Use std::map to record each different vertex/normal pairs
    std::map<std::tuple<GLuint, GLuint>, GLuint> indices_map;
    for (auto it_f = faces.begin(); it_f != faces.end(); it_f += 2) {
        GLuint v = *it_f, n = *(it_f + 1);
        auto[it_m, inserted] = indices_map.insert(std::make_pair(std::make_tuple(v, n), attrs.size() >> 1));
        if (inserted) {
            attrs.push_back(vertices[v]);
            attrs.push_back(normals[n]);
        }
        indices.push_back(it_m->second);
    }
    std::cout << objFilename << " " << (attrs.size() >> 1) << std::endl;

    return Mesh(attrs, indices);
}


Mesh Mesh::cube() {
    std::array<GLfloat, 6 * 6 * 6> data = {
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
            1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
            -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,

            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

            -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
            -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,

            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

            -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
            1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,

            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
    };

    std::vector<glm::vec3> attrs;
    for (size_t i = 0; i < data.size(); i += 3) {
        attrs.emplace_back(data[i], data[i + 1], data[i + 2]);
    }

    std::vector<GLuint> indices(6 * 6);
    std::iota(indices.begin(), indices.end(), 0);

    return Mesh(attrs, indices);
}
