#ifndef SKYBOX_H
#define SKYBOX_H


#include "Node.h"
#include "../components/geometries/Mesh.h"

// Skybox does not honor Node::children, Node::components, Node::transform
// Node::copy will not work
class Skybox : public Node {
public:
    explicit Skybox();

    ~Skybox() override;

    void
    draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) override;

    bool cull(const glm::mat4 &view_proj) override;

private:
    std::shared_ptr<Mesh> cube;

    std::shared_ptr<Shader> shader;

    GLuint cubeMap = 0;
};


#endif //SKYBOX_H
