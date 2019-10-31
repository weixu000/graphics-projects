#include "Wireframe.h"

std::unique_ptr<Shader> Wireframe::shader;

Wireframe::Wireframe() {
    if (!shader) {
        shader = std::make_unique<Shader>("shaders/plain.vert", "shaders/plain.frag");
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

    // Generate VAO, VBO.
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Bind to the VAO.
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
                 vertices.data(), GL_STATIC_DRAW);
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

Wireframe::~Wireframe() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

Wireframe::Wireframe(Wireframe &&other) : Wireframe() {
    *this = std::move(other);
}

Wireframe &Wireframe::operator=(Wireframe &&other) {
    count = other.count;
    vbo = other.vbo;
    vao = other.vao;
    ebo = other.ebo;

    other.count = 0;
    other.vbo = other.vao = other.ebo = 0;

    return *this;
}

void Wireframe::draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) {
    shader->use();
    shader->setUniformMatrix4("projection", projection);
    shader->setUniformMatrix4("view", view);
    shader->setUniformMatrix4("model", world);
    // Bind to the VAO.
    glBindVertexArray(vao);
    // Draw points
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Unbind from the VAO.
    glBindVertexArray(0);
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
