#ifndef FRAME_BUFFER_OBJECT_H
#define FRAME_BUFFER_OBJECT_H

#include <glad/glad.h>
#include <glm/detail/setup.hpp>

class FrameBufferObject
{
public:
    FrameBufferObject(GLsizei width, GLsizei height);
    
    void BindTexture(GLenum textureUnit);
    void Bind() const;
    void Unbind();
    void Delete();
    
private:
    GLuint ID;
    glm::uint depthTexture;
};

#endif