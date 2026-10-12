#version 330 core

in vec2 texCoord;

uniform sampler2D diffuse0;

void main()
{
    if (texture(diffuse0, texCoord).a < 0.5)
            discard;
    
    
}