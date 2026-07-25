#version 330 core

out vec4 FragColor;

//Imports from the vertex shader
in vec3 crrPos;
in vec3 normal;
in vec2 texCoord;

//Imports:
uniform sampler2D diffuse0;
uniform vec3 lightColor;
uniform vec3 directionalLight;

vec4 DirectionalLight();

void main()
{
    FragColor = DirectionalLight();
}

vec4 DirectionalLight()
{
    //ambient lighting
    float ambient = 0.2f;
    
    //Diffuse lighting
    vec3 normalizedNormal = normalize(normal);
    vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
    float diffuse = max(dot(normalizedNormal, lightDirection), 0.0f);
    
    vec4 textureColor = (texture(diffuse0, texCoord) * (diffuse + ambient)) * lightColor;
    
    if (textureColor.a < 0.5)
            discard;
    
    return textureColor;
}