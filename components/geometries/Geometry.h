#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include <memory>

#include "../Component.h"

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
    std::array<glm::vec3, 8> vertices{};
};


class Geometry : public Component {
public:
    virtual AABB boundingBox() const = 0;

    bool cull(const glm::mat4 &view_proj) override;
};

#endif

