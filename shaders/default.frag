#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool useGrid;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0)
        return 0.0;

    // Calculate bias based on surface angle
    float bias = max(0.003 * (1.0 - dot(normal, lightDir)), 0.001);

    // Percentage-Closer Filtering (PCF) soft shadows
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // Ambient
    vec3 ambient = material.ambient * lightColor;

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse * lightColor;

    // Specular (Blinn-Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * material.specular * lightColor;

    // Grid effect for platform surface
    if (useGrid) {
        vec2 coord = FragPos.xz;
        vec2 grid = abs(fract(coord - 0.5) - 0.5) / (fwidth(coord) + 0.001);
        float line = min(grid.x, grid.y);
        float c = 1.0 - min(line, 1.0);
        vec3 baseDiffuse = mix(material.diffuse, vec3(0.12, 0.15, 0.20), c * 0.7);
        diffuse = diff * baseDiffuse * lightColor;
    }

    // Shadow calculation
    float shadow = ShadowCalculation(FragPosLightSpace, norm, lightDir);
    vec3 result = ambient + (1.0 - shadow) * (diffuse + specular);

    FragColor = vec4(result, 1.0);
}
