#version 330 core

out vec4 FragColor;

//Imports from the vertex shader
in vec2 texCoord;
in vec3 normal;
in vec3 vertexTint;
in vec3 worldPosition;
in vec4 lightSpacePosition;

flat in float vertexUnlit;

//Imports:
uniform sampler2D diffuse0;
uniform sampler2D shadowMap;

uniform vec3 lightColor;
uniform vec3 directionalLight;
uniform vec3 cameraPosition;

float CalculateShadow(vec3 N, vec3 L);

void main()
{
    vec4 albedo = texture(diffuse0, texCoord);

    if (albedo.a < 0.5)
        discard;
    
    vec3 N = normalize(normal);
    vec3 L = normalize(-directionalLight);
    
    float diffuse = max(dot(N, L), 0.0);
    
    if (vertexUnlit > 0.5)
        diffuse = 1.0;
    
    float shadow = CalculateShadow(N, L);
    
    vec3 ambient = vec3(0.8, 0.9, 1.0) * 0.5;
    vec3 sunlight = lightColor * 0.9 * diffuse * (1.0 - shadow);
    vec3 baseColor = albedo.rgb * vertexTint;
    
    FragColor = vec4(baseColor * (ambient + sunlight), albedo.a);
}

float CalculateShadow(vec3 N, vec3 L)
{
    vec3 projected = lightSpacePosition.xyz / lightSpacePosition.w;
    projected = projected * 0.5 + 0.5;

    if (projected.x <= 0.0 || projected.x >= 1.0 ||
    projected.y <= 0.0 || projected.y >= 1.0 ||
    projected.z <= 0.0 || projected.z >= 1.0)
    {
        return 0.0;
    }

    float facing = max(dot(N, L), 0.0);
    float bias = max(0.0002 * (1.0 - facing), 0.00004);

    if (vertexUnlit > 0.5)
    bias = 0.0001;

    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    float shadow = 0.0;

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            float nearestDepth = texture(shadowMap, projected.xy + vec2(x, y) * texelSize).r;
            shadow += projected.z - bias > nearestDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    float distanceFromCamera = length(worldPosition - cameraPosition);
    float fade = 1.0 - smoothstep(384.0, 480.0, distanceFromCamera);

    return shadow * fade;
}