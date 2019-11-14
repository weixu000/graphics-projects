#include <cmath>

#include "BezierCurve.h"

std::unique_ptr<Shader> BezierCurve::shader;

BezierCurve::BezierCurve() {
    if (!shader) {
        shader = std::make_unique<Shader>("shaders/flat.vert", "shaders/flat.frag",
                                          "shaders/bezier.geom");
    }
}

void BezierCurve::upload() {
    assert(controlPoints.size() % 3 == 0 || (controlPoints.size() % 3 == 1 && controlPoints.size() / 3 > 0));

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
        indices[4 * i + 3] = (3 * i + 3) % controlPoints.size();
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

glm::mat4 BezierCurve::B(-1, 3, -3, 1,
                         3, -6, 3, 0,
                         -3, 3, 0, 0,
                         1, 0, 0, 0);

std::tuple<int, float> BezierCurve::segment(float t) {
    auto i = int(std::floor(t));
    t -= i;
    i = std::div(i, controlPoints.size() / 3).rem;
    if (i < 0) {
        i += controlPoints.size() / 3;
    }
    return std::make_tuple(i, t);
}

glm::vec3 BezierCurve::position(float t) {
    auto[i, t_]=segment(t);
    glm::mat4 G(glm::vec4(controlPoints[3 * i], 1.0f),
                glm::vec4(controlPoints[3 * i + 1], 1.0f),
                glm::vec4(controlPoints[3 * i + 2], 1.0f),
                glm::vec4(controlPoints[(3 * i + 3) % controlPoints.size()], 1.0f));

    return G * B * glm::vec4(t_ * t_ * t_, t_ * t_, t_, 1.0);
}

glm::vec3 BezierCurve::derivative(float t) {
    auto[i, t_]=segment(t);
    glm::mat4 G(glm::vec4(controlPoints[3 * i], 1.0f),
                glm::vec4(controlPoints[3 * i + 1], 1.0f),
                glm::vec4(controlPoints[3 * i + 2], 1.0f),
                glm::vec4(controlPoints[(3 * i + 3) % controlPoints.size()], 1.0f));

    return G * B * glm::vec4(3 * t_ * t_, 2 * t_, 1, 0.0);
}
