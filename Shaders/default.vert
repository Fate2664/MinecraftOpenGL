#version 330 core

//Position
layout (location = 0) in vec3 aPos;
//Texture
layout (location = 1) in vec2 aTex;
//Normal
layout (location = 2) in vec3 aNormal;
//Tint
layout (location = 3) in vec3 aTint;

//Exports:
out vec2 texCoord;
out vec3 normal;
out vec3 vertexTint;

//Imports
uniform mat4 camMatrix;
uniform mat4 chunk;

void main()
{
    vec3 worldPosition = vec3(chunk * vec4(aPos, 1.0f));
    vertexTint = aTint;
    texCoord = aTex;
    normal = normalize(mat3(transpose(inverse(chunk))) * aNormal);
  
    gl_Position = camMatrix * vec4(worldPosition, 1.0);
}