#include "PointCloud.h"

#include <fstream>
#include <sstream>
#include <iostream>

PointCloud::PointCloud(const std::string &objFilename, GLfloat pointSize)
        : pointSize(pointSize) {
    /*
     * Read points from an obj file.
     */
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
            }
        }
        std::cout << objFilename << " " << points.size() << std::endl;
    } else {
        std::cerr << "Can't open the file " << objFilename << std::endl;
    }
    objFile.close();

    /*
     * Normalize the object to fit in the screen.
     */
    glm::vec3 minVal = points[0], maxVal = points[0];
    for (auto &p:points) {
        minVal = glm::min(minVal, p);
        maxVal = glm::max(maxVal, p);
    }
    glm::vec3 center = (maxVal + minVal) / 2.0f;

    float scale = 0.0f;
    for (auto &p:points) {
        scale = glm::max(glm::length(p - center), scale);
    }

    model = glm::mat4(1);
    model = glm::scale(model, glm::vec3(11.5 / scale));
    model = glm::translate(model, -center);

    // Set the color.
    color = glm::vec3(1, 0, 0);

    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Bind to the VAO.
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
                 points.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

PointCloud::~PointCloud() {
    // Delete the VBO and the VAO.
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void PointCloud::draw(Shader &) {
    // Bind to the VAO.
    glBindVertexArray(vao);
    // Set point size.
    glPointSize(pointSize);
    // Draw points
    glDrawArrays(GL_POINTS, 0, points.size());
    // Unbind from the VAO.
    glBindVertexArray(0);
}

void PointCloud::update() {
    // Spin the cube by 1 degree.
    spin(0.1f);
}

void PointCloud::updatePointSize(GLfloat delta) {
    pointSize = std::max(1.0f, std::min(50.0f, pointSize + delta));
}

void PointCloud::spin(float deg) {
    // Update the model matrix by multiplying a rotation matrix
    model = glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f)) * model;
//    model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

