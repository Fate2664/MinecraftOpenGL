#version 330 core

out vec4 FragColor;

//Imports from the vertex shader
in vec3 crrPos;
in vec3 normal;
in vec2 texCoord;

//Imports:
uniform sampler2D diffuse0;
uniform vec3 lightColor;
//Direction the sunlight travels
uniform vec3 directionalLight;

vec4 DirectionalLight(vec3 directionalLight, vec3 lightColor);

void main()
{
    FragColor = DirectionalLight(directionalLight, lightColor);
}

vec4 DirectionalLight(vec3 directionalLight, vec3 lightColor)
{
    vec4 albedo = texture(diffuse0, texCoord);
    
    if (albedo.a < 0.5)
            discard;
    
    vec3 normalizedNormal = normalize(normal);
    vec3 lightDirection = normalize(-directionalLight);
    
    //Diffuse lighting
    float ambientStrength = 0.35f;
    float diffuseStrength = max(dot(normalizedNormal, lightDirection), 0.0f);
    float brightness = ambientStrength + diffuseStrength * 0.9f;
    
    return vec4(albedo.rgb * lightColor * brightness, albedo.a);
}