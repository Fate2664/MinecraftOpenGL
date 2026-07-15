#ifndef ELEMENT_BUFFER_OBJECT_CLASS_H
#define ELEMENT_BUFFER_OBJECT_CLASS_H

#include <glad/glad.h>
#include <vector>

class ElementBufferObject {
public:
	GLuint ID;
	ElementBufferObject(std::vector<GLuint>& indices);

	void Bind();
	void Unbind();
	void Delete();
};


#endif