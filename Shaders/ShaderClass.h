#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <glm/fwd.hpp>

std::string readFile(const char* fileName);

class Shader {
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);
	
	void SetMat4(const char* name, const glm::mat4& value);
	void SetVec4(const char* name, const glm::vec4& value);
	void SetVec3(const char* name, const glm::vec3& value);
	void SetFloat(const char* name, const float value);
	void SetInt(const char* name, const int value);
	
	void Activate();
	void Delete();
private:
	void compileErrors(unsigned int shader, const char* type);
};

#endif