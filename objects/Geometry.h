#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <memory>

#include "Node.h"

class AABB {
public:
    AABB() = default;

    AABB(const glm::vec3 &minVal, const glm::vec3 &maxVal);

    /*
     *    4----7
     *   /|   /|
     *  5-+--6 |
     *  | 0--+-3
     *  |/   |/
     *  1----2
     *
     */
    std::array<glm::vec3, 8> vertices;
};


class Geometry : public Node {
public:
    virtual AABB boundingBox() const = 0;

protected:
    std::shared_ptr<Shader> shader;
};

#endif

