#ifndef ROBOT_H
#define ROBOT_H

#include "Geometry.h"
#include "Transform.h"
#include "Mesh.h"


class Robot : public Geometry {
public:
    Robot();

    void draw(const glm::mat4 &world) override;

    void useShader(const std::shared_ptr<Shader> &s) override;

private:
    std::shared_ptr<Transform> root;

    std::shared_ptr<Mesh> antenna, eyeball, head, body, limb;

    void initHead();

    void initArms();

    void initLegs();
};


#endif //ROBOT_H
