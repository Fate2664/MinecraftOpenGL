#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject() {
	glGenVertexArrays(1, &ID);
}

void VertexArrayObject::LinkAttrib(VertexBufferObject& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}


void VertexArrayObject::Bind() {
	glBindVertexArray(ID);
}

void VertexArrayObject::Unbind() {
	glBindVertexArray(0);
}

void VertexArrayObject::Delete() {
	glDeleteVertexArrays(1, &ID);
}