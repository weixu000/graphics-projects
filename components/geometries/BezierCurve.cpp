#include "BezierCurve.h"

std::unique_ptr<Shader> BezierCurve::shader;

BezierCurve::BezierCurve() {
    if (!shader) {
        shader = std::make_unique<Shader>("shaders/plain.vert", "shaders/plain.frag",
                                          "shaders/bezier.geom");
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

BezierCurve::~BezierCurve() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &ebo);
}

BezierCurve::BezierCurve(BezierCurve &&other) {
    *this = std::move(other);
}

BezierCurve &BezierCurve::operator=(BezierCurve &&other) {
    controlPoints = std::move(other.controlPoints);

    vbo = other.vbo;
    vao = other.vao;
    ebo = other.ebo;

    other.vbo = other.vao = other.ebo = 0;

    return *this;
}

void BezierCurve::upload() {
    assert(controlPoints.empty() || (controlPoints.size() % 3 == 1 && controlPoints.size() / 3 > 0));

    const auto inf = std::numeric_limits<float>::infinity();
    glm::vec3 minVal(inf, inf, inf), maxVal(-inf, -inf, -inf);
    for (auto &v:controlPoints) {
        minVal = glm::min(minVal, v);
        maxVal = glm::max(maxVal, v);
    }
    bb = {minVal, maxVal};

    std::vector<GLuint> indices(controlPoints.size() / 3 * 4);
    for (size_t i = 0; i < controlPoints.size() / 3; ++i) {
        indices[4 * i] = 3 * i;
        indices[4 * i + 1] = 3 * i + 1;
        indices[4 * i + 2] = 3 * i + 2;
        indices[4 * i + 3] = 3 * i + 3;
    }

    // Bind to the VAO.
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * controlPoints.size(),
                 controlPoints.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(glm::vec3), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // Pass in the data.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(),
                 indices.data(), GL_STATIC_DRAW);

    // Unbind from the VAO.
    glBindVertexArray(0);
}

void
BezierCurve::draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) {
    shader->use();
    shader->setUniformMatrix4("projection", projection);
    shader->setUniformMatrix4("view", view);
    shader->setUniformMatrix4("model", world);
    shader->setUniform1i("nSamples", 80);
    // Bind to the VAO.
    glBindVertexArray(vao);
    // Draw lines
    glDrawElements(GL_LINES_ADJACENCY, controlPoints.size() / 3 * 4, GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}
