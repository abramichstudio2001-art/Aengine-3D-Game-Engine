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

// ACES Filmic Tone Mapping Curve
vec3 ACESFilm(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0)
        return 0.0;

    float bias = max(0.002 * (1.0 - dot(normal, lightDir)), 0.0008);

    // 16-sample PCF soft shadow calculation
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 2; ++x) {
        for (int y = -1; y <= 2; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize * 0.85).r;
            shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 16.0;

    return shadow;
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Directional sky & ground ambient tint
    float up = dot(norm, vec3(0.0, 1.0, 0.0)) * 0.5 + 0.5;
    vec3 skyColor = vec3(0.25, 0.32, 0.45) * 1.2;
    vec3 groundColor = vec3(0.08, 0.07, 0.06);
    vec3 ambient = mix(groundColor, skyColor, up) * material.ambient * lightColor * 1.3;

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse * lightColor * 1.4;

    // Specular (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * material.specular * lightColor * 1.2;

    // Rim Lighting Effect for visual pop
    float rim = 1.0 - max(dot(viewDir, norm), 0.0);
    rim = pow(rim, 3.0);
    vec3 rimColor = vec3(0.4, 0.5, 0.7) * rim * diff * 0.35;

    // Anti-Aliased Grid Effect for Platform
    if (useGrid) {
        vec2 coord = FragPos.xz * 1.0;
        vec2 grid = abs(fract(coord - 0.5) - 0.5) / (fwidth(coord) + 0.0001);
        float line = min(grid.x, grid.y);
        float gridPattern = 1.0 - clamp(line, 0.0, 1.0);

        vec3 gridLineColor = vec3(0.20, 0.28, 0.42) * 1.5;
        vec3 baseDiffuse = mix(material.diffuse, gridLineColor, gridPattern * 0.85);
        diffuse = diff * baseDiffuse * lightColor * 1.4;
    }

    // Shadow Calculation
    float shadow = ShadowCalculation(FragPosLightSpace, norm, lightDir);
    vec3 result = ambient + (1.0 - shadow) * (diffuse + specular) + rimColor;

    // High Quality Post-Processing: ACES Filmic Tone Mapping & Gamma Correction
    result = ACESFilm(result);
    result = pow(result, vec3(1.0 / 2.2));

    FragColor = vec4(result, 1.0);
}
