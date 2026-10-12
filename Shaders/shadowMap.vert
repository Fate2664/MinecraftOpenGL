#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 texCoord;

uniform mat4 lightProjection;
uniform mat4 chunk;

void main()
{
    texCoord = aTex;
    gl_Position = lightProjection * chunk * vec4(aPos, 1.0);    
}