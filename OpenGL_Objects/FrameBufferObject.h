#ifndef FRAME_BUFFER_OBJECT_H
#define FRAME_BUFFER_OBJECT_H

#include <glad/glad.h>
#include <glm/detail/setup.hpp>

class FrameBufferObject
{
public:
    GLuint ID;
    glm::uint frameBufferTexture;
    glm::uint RBO;
    FrameBufferObject();
    
    void LinkAttrib();
    void GenerateTexture();
    void Bind();
    void Unbind();
    void Delete();
};

#endif