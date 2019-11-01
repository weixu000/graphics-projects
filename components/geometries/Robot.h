#ifndef ROBOT_H
#define ROBOT_H

#include "../../objects/Node.h"
#include "Geometry.h"
#include "../Transform.h"
#include "Mesh.h"


class Robot : public Geometry {
public:
    Robot(std::shared_ptr<Transform> headCtl = std::make_shared<Transform>(),
          std::shared_ptr<Transform> leftArmCtl = std::make_shared<Transform>(),
          std::shared_ptr<Transform> rightArmCtl = std::make_shared<Transform>(),
          std::shared_ptr<Transform> leftLegCtl = std::make_shared<Transform>(),
          std::shared_ptr<Transform> rightLegCtl = std::make_shared<Transform>());

    void
    draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) override;

    void useShader(const std::shared_ptr<Shader> &s);

    AABB boundingBox() const override { return bb; }

private:
    Node root;

    std::shared_ptr<Mesh> antenna, eyeball, head, body, limb;

    const AABB bb{glm::vec3(-1.6f, -2.2f, -1.5f), glm::vec3(1.6f, 2.5f, 1.5f)};

    void initHead(std::shared_ptr<Transform> headCtl);

    void initArms(std::shared_ptr<Transform> leftCtl, std::shared_ptr<Transform> rightCtl);

    void initLegs(std::shared_ptr<Transform> leftCtl, std::shared_ptr<Transform> rightCtl);
};


#endif //ROBOT_H
