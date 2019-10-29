#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <memory>

#include "../shaders/Shader.h"
#include "Node.h"

class Geometry : public Node {
public:
    virtual void useShader(const std::shared_ptr<Shader> &s) { shader = s; }

protected:
    std::shared_ptr<Shader> shader;
};

#endif

