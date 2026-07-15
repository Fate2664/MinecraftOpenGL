#version 330 core

out vec4 FragColor;

in vec3 crrPos;
in vec2 texCoord;

//Imports:
uniform sampler2D diffuse0;
uniform vec3 camPos;

void main()
{
    vec4 textureColor = texture(diffuse0, texCoord);
    
    if(textureColor.a < 0.5)
        discard;
    
    FragColor = textureColor;
}