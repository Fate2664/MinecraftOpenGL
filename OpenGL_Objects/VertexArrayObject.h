#ifndef VERTEX_ARRAY_OBJECT_CLASS_H
#define VERTEX_ARRAY_OBJECT_CLASS_H

#include <glad/glad.h>
#include "VertexBufferObject.h"

class VertexArrayObject {
public: GLuint ID;
	  VertexArrayObject();

	  void LinkAttrib(VertexBufferObject& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	  void Bind();
	  void Unbind();
	  void Delete();
};

#endif