#ifndef CAMERA_H
#define CAMERA_H

#include <glm/gtx/transform.hpp>

#include "Node.h"

class Camera : public Node {
public:
    glm::mat4 projection, view;
    glm::vec3 eye;

    explicit Camera(const glm::mat4 &p, const glm::mat4 &t = glm::mat4(1.0f));

    explicit Camera(const glm::mat4 &p, std::shared_ptr<Transform> t);

    void update() override;

    std::unique_ptr<Node> copy() override;

    // Camera matrix, inverse of view matrix glm::lookAt
    static glm::mat4 orientation(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up);

protected:
    void doCopy(Camera *dup);
};


#endif //CAMERA_H
