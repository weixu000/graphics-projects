#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "Geometry.h"
#include "../../shaders/Shader.h"

class BezierCurve : public Geometry {
public:
    BezierCurve();

    ~BezierCurve() override;

    BezierCurve(const BezierCurve &) = delete;

    BezierCurve(BezierCurve &&);

    BezierCurve &operator=(const BezierCurve &) = delete;

    BezierCurve &operator=(BezierCurve &&);

    void
    draw(const glm::mat4 &world, const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &eye) override;

    std::vector<glm::vec3> controlPoints;

    void upload();

    AABB boundingBox() const override { return bb; }

private:
    GLuint vao = 0, vbo = 0, ebo = 0;

    AABB bb;

    static std::unique_ptr<Shader> shader;
};


#endif //BEZIERCURVE_H
