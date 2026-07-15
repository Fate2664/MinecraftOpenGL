#version 330 core

//Position
layout (location = 0) in vec3 aPos;
//Texture
layout (location = 1) in vec2 aTex;


//Exports:
out vec3 crrPos;
out vec2 texCoord;


//Imports
uniform mat4 camMatrix;
uniform mat4 chunk;

void main()
{
    crrPos = vec3(chunk * vec4(aPos, 1.0f));
    texCoord = aTex;
  
    gl_Position = camMatrix * vec4(crrPos, 1.0);
}