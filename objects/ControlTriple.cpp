#include "ControlTriple.h"

std::shared_ptr<Shader> ControlTriple::shader;
std::shared_ptr<Mesh> ControlTriple::controlIndicator, ControlTriple::approxIndicator;

ControlTriple::ControlTriple(glm::vec3 *left, glm::vec3 *middle, glm::vec3 *right)
        : left(left), middle(middle), right(right) {
    if (!shader || !controlIndicator || !approxIndicator) {
        shader = std::make_shared<Shader>("shaders/flat.vert", "shaders/flat.frag");
        controlIndicator = approxIndicator = std::make_shared<Mesh>(Mesh::fromObjFile("meshes/sphere.obj"));
        controlIndicator->useShader(shader);
        approxIndicator->useShader(shader);
    }
    setMiddle(*middle);
}

void ControlTriple::setLeft(const glm::vec3 &val) {
    auto delta = *middle - val;
    *left = *middle - delta;
    *right = *middle + delta;
    upload();
}

void ControlTriple::setRight(const glm::vec3 &val) {
    auto delta = val - *middle;
    *left = *middle - delta;
    *right = *middle + delta;
    upload();
}

void ControlTriple::setMiddle(const glm::vec3 &val) {
    auto delta = (*right - *left) / 2.0f;
    *middle = val;
    *left = *middle - delta;
    *right = *middle + delta;
    upload();
}

void ControlTriple::draw(const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) {
    shader->use();
    shader->setUniform3f("color", glm::vec3(0.0f, 0.0f, 1.0f));
    shader->setUniformMatrix4("projection", projection);
    shader->setUniformMatrix4("view", view);
    shader->setUniformMatrix4("model", glm::mat4(1.0f));
    vao.bind();
    glDrawArrays(GL_LINES, 0, 2);
    vao.unbind();

    shader->setUniform3f("color", glm::vec3(1.0f, 0.0f, 0.0f));
    controlIndicator->draw(glm::translate(*middle) * glm::scale(glm::vec3(0.05f)), projection, view, eye);

    shader->setUniform3f("color", glm::vec3(0.0f, 1.0f, 0.0f));
    approxIndicator->draw(glm::translate(*left) * glm::scale(glm::vec3(0.05f)), projection, view, eye);
    approxIndicator->draw(glm::translate(*right) * glm::scale(glm::vec3(0.05f)), projection, view, eye);
}

void ControlTriple::upload() {
    // Bind to the VAO.
    vao.bind();

    std::array<glm::vec3, 2> segments = {*left, *right};

    // Pass in the data.
    vbo.upload(sizeof(glm::vec3) * segments.size(), segments.data());
    // Enable vertex attribute 0.
    vao.setAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                         sizeof(glm::vec3));
    vbo.unbind();

    // Unbind from the VAO.
    vao.unbind();
}
