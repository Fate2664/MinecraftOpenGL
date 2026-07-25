#ifndef VERTEX_BUFFER_OBJECT_CLASS_H
#define VERTEX_BUFFER_OBJECT_CLASS_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

struct Vertex 
{
	glm::vec3 position;
	glm::vec2 texUV;
	glm::vec3 normal;
};

class VertexBufferObject {
public:
	GLuint ID;
	VertexBufferObject(std::vector<Vertex>& vertices);

	void Bind();
	void Unbind();
	void Delete();
};

#endif