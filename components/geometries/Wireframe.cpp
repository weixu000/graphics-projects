#include "Wireframe.h"

std::unique_ptr<Shader> Wireframe::shader;

Wireframe::Wireframe() {
    if (!shader) {
        shader = std::make_unique<Shader>("shaders/flat.vert", "shaders/flat.frag");
    }
}

Wireframe::Wireframe(const std::vector<glm::vec3> &vertices, const std::vector<GLuint> &indices)
        : Wireframe() {
    count = indices.size();

    const auto inf = std::numeric_limits<float>::infinity();
    glm::vec3 minVal(inf, inf, inf), maxVal(-inf, -inf, -inf);
    for (auto &v:vertices) {
        minVal = glm::min(minVal, v);
        maxVal = glm::max(maxVal, v);
    }
    bb = {minVal, maxVal};

    // Bind to the VAO.
    vao.bind();

    // Pass in the data.
    vbo.upload(sizeof(glm::vec3) * vertices.size(), vertices.data());
    // Enable vertex attribute 0.
    vao.setAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                         sizeof(glm::vec3));
    vbo.unbind();

    ebo.bind(GL_ELEMENT_ARRAY_BUFFER);
    // Pass in the data.
    ebo.upload(sizeof(GLuint) * indices.size(), indices.data(), GL_ELEMENT_ARRAY_BUFFER);

    // Unbind from the VAO.
    vao.unbind();
}

void Wireframe::draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) {
    shader->use();
    shader->setUniformMatrix4("projection", projection);
    shader->setUniformMatrix4("view", view);
    shader->setUniformMatrix4("model", world);
    shader->setUniform3f("color", glm::vec3(1.0f, 1.0f, 1.0f));
    // Bind to the VAO.
    vao.bind();
    // Draw points
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    // Unbind from the VAO.
    vao.unbind();
}

Wireframe Wireframe::fromAABB(const AABB &bb) {
    std::vector<glm::vec3> vertices(bb.vertices.begin(), bb.vertices.end());
    std::vector<GLuint> indices{
            0, 2, 1, 0, 3, 2,
            0, 1, 4, 1, 5, 4,
            0, 7, 3, 0, 4, 7,
            0, 2, 1, 0, 3, 2,
            1, 2, 5, 2, 6, 5,
            2, 3, 6, 3, 7, 6,
            4, 5, 6, 4, 6, 7
    };
    return Wireframe(vertices, indices);
}
