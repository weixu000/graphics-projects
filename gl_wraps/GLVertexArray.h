#ifndef GLVERTEXARRAY_H
#define GLVERTEXARRAY_H

#include "GLObject.h"

inline GLuint createVertexArray() {
    GLuint id;
    glGenVertexArrays(1, &id);
    return id;
}

inline void delVertexArray(GLuint id) { glDeleteVertexArrays(1, &id); }

class GLVertexArray : public GLObject<createVertexArray, delVertexArray> {
public:
    void bind() { glBindVertexArray(id); }

    static void unbind() { glBindVertexArray(0); }

    void setAttribPointer(GLuint index,
                          GLint size, GLenum type, GLboolean normalized,
                          GLsizei stride, GLsizei pointer = 0) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, normalized,
                              stride, reinterpret_cast<const void *>(pointer));
    }
};

#endif //GLVERTEXARRAY_H
