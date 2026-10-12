#version 330 core

//Position
layout (location = 0) in vec3 aPos;
//Texture
layout (location = 1) in vec2 aTex;
//Normal
layout (location = 2) in vec3 aNormal;
//Tint
layout (location = 3) in vec3 aTint;
//Unlit
layout (location = 4) in float aUnlit;


//Exports:
out vec2 texCoord;
out vec3 normal;
out vec3 vertexTint;
out vec3 worldPosition;
out vec4 lightSpacePosition;

flat out float vertexUnlit;

//Imports
uniform mat4 camMatrix;
uniform mat4 chunk;
uniform mat4 lightSpaceMatrix;

void main()
{
    vec4 world = chunk * vec4(aPos, 1.0f);
    
    worldPosition = world.xyz;
    lightSpacePosition = lightSpaceMatrix * world;
    
    vertexTint = aTint;
    texCoord = aTex;
    vertexUnlit = aUnlit;
    
    normal = normalize(mat3(transpose(inverse(chunk))) * aNormal);
    gl_Position = camMatrix * world;
}