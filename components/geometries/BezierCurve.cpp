#include "BezierCurve.h"

std::unique_ptr<Shader> BezierCurve::shader;

BezierCurve::BezierCurve() {
    if (!shader) {
        shader = std::make_unique<Shader>("shaders/flat.vert", "shaders/flat.frag",
                                          "shaders/bezier.geom");
    }
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
    vao.bind();

    // Pass in the data.
    vbo.upload(sizeof(glm::vec3) * controlPoints.size(), controlPoints.data());
    // Enable vertex attribute 0.
    vao.setAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                         sizeof(glm::vec3));
    vbo.unbind();

    ebo.bind(GL_ELEMENT_ARRAY_BUFFER);
    ebo.upload(sizeof(GLuint) * indices.size(), indices.data(), GL_ELEMENT_ARRAY_BUFFER);

    // Unbind from the VAO.
    vao.unbind();
}

void
BezierCurve::draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) {
    shader->use();
    shader->setUniformMatrix4("projection", projection);
    shader->setUniformMatrix4("view", view);
    shader->setUniformMatrix4("model", world);
    shader->setUniform1i("nSamples", 80);
    shader->setUniform3f("color", glm::vec3(1.0f, 1.0f, 1.0f));
    // Bind to the VAO.
    vao.bind();
    // Draw lines
    glDrawElements(GL_LINES_ADJACENCY, controlPoints.size() / 3 * 4, GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    vao.unbind();
}
