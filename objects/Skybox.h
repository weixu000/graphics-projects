#ifndef SKYBOX_H
#define SKYBOX_H


#include "../components/geometries/Mesh.h"

class Skybox {
public:
    explicit Skybox();

    virtual ~Skybox();

    void
    draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye);

    GLuint cubeMap = 0;

private:
    std::shared_ptr<Mesh> cube;

    std::shared_ptr<Shader> shader;
};


#endif //SKYBOX_H
