#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 lightProjection;
uniform mat4 chunk;

void main()
{
    gl_Position = lightProjection * chunk * vec4(aPos, 1.0);    
}