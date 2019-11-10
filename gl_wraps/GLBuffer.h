#ifndef GLBUFFER_H
#define GLBUFFER_H

#include "GLObject.h"

inline GLuint createBuffer() {
    GLuint id;
    glGenBuffers(1, &id);
    return id;
}

inline void delBuffer(GLuint id) { glDeleteBuffers(1, &id); }

class GLBuffer : public GLObject<createBuffer, delBuffer> {
public:
    GLBuffer() = default;

    GLBuffer(GLsizeiptr size, const void *data,
             GLenum target = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW) {
        upload(size, data, target, usage);
        glBindBuffer(target, 0);
    }

    void upload(GLsizeiptr size, const void *data,
                GLenum target = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW) {
        glBindBuffer(target, id);
        glBufferData(target, size, data, usage);
    }

    void bind(GLenum target = GL_ARRAY_BUFFER) { glBindBuffer(target, id); }

    static void unbind(GLenum target = GL_ARRAY_BUFFER) { glBindBuffer(target, 0); }
};

#endif //GLBUFFER_H
