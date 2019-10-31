#ifndef ROBOT_H
#define ROBOT_H

#include "../../Node.h"
#include "Geometry.h"
#include "../Transform.h"
#include "Mesh.h"


class Robot : public Geometry {
public:
    Robot();

    void
    draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) override;

    void update() override;

    void useShader(const std::shared_ptr<Shader> &s);

    AABB boundingBox() const override { return bb; }

private:
    Node root;

    std::shared_ptr<Mesh> antenna, eyeball, head, body, limb;

    std::shared_ptr<Transform> headControl, leftArmControl, rightArmControl, leftLegControl, rightLegControl;

    const AABB bb{glm::vec3(-1.6f, -2.2f, -1.5f), glm::vec3(1.6f, 2.5f, 1.5f)};

    void initHead();

    void initArms();

    void initLegs();
};


#endif //ROBOT_H
