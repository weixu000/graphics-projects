#include "ControlTriple.h"

std::shared_ptr<Shader> ControlTriple::shader;
std::shared_ptr<Mesh> ControlTriple::controlIndicator, ControlTriple::approxIndicator;

ControlTriple::ControlTriple(glm::vec3 *left, glm::vec3 *middle, glm::vec3 *right) {
    points = {left, middle, right};
    if (!shader || !controlIndicator || !approxIndicator) {
        shader = std::make_shared<Shader>("shaders/flat.vert", "shaders/flat.frag");
        controlIndicator = approxIndicator = std::make_shared<Mesh>(Mesh::fromObjFile("meshes/sphere.obj"));
        controlIndicator->useShader(shader);
        approxIndicator->useShader(shader);
    }
    set(1, *middle);
}

void ControlTriple::set(int i, const glm::vec3 &val) {
    assert(i < points.size());
    switch (i) {
        case 0: {
            auto delta = *points[1] - val;
            *points[0] = *points[1] - delta;
            *points[2] = *points[1] + delta;
        }
            break;
        case 1: {
            auto delta = (*points[2] - *points[0]) / 2.0f;
            *points[1] = val;
            *points[0] = *points[1] - delta;
            *points[2] = *points[1] + delta;
        }
            break;
        case 2: {
            auto delta = val - *points[1];
            *points[0] = *points[1] - delta;
            *points[2] = *points[1] + delta;
        }
            break;
    }
    upload();
}

void ControlTriple::draw(const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye, GLint stencil) {
    shader->use();
    shader->setUniform3f("color", glm::vec3(0.0f, 0.0f, 1.0f));
    shader->setUniformMatrix4("projection", projection);
    shader->setUniformMatrix4("view", view);
    shader->setUniformMatrix4("model", glm::mat4(1.0f));
    vao.bind();
    glDrawArrays(GL_LINES, 0, 2);
    vao.unbind();

    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    shader->setUniform3f("color", glm::vec3(1.0f, 0.0f, 0.0f));
    glStencilFunc(GL_ALWAYS, stencil + 1, 0xFF);
    controlIndicator->draw(glm::translate(*points[1]) * glm::scale(glm::vec3(0.05f)), projection, view, eye);

    shader->setUniform3f("color", glm::vec3(0.0f, 1.0f, 0.0f));
    glStencilFunc(GL_ALWAYS, stencil, 0xFF);
    approxIndicator->draw(glm::translate(*points[0]) * glm::scale(glm::vec3(0.05f)), projection, view, eye);
    glStencilFunc(GL_ALWAYS, stencil + 2, 0xFF);
    approxIndicator->draw(glm::translate(*points[2]) * glm::scale(glm::vec3(0.05f)), projection, view, eye);

    glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
}

void ControlTriple::upload() {
    // Bind to the VAO.
    vao.bind();

    std::array<glm::vec3, 2> segments = {*points[0], *points[2]};

    // Pass in the data.
    vbo.upload(sizeof(glm::vec3) * segments.size(), segments.data());
    // Enable vertex attribute 0.
    vao.setAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                         sizeof(glm::vec3));
    vbo.unbind();

    // Unbind from the VAO.
    vao.unbind();
}
