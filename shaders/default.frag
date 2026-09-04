#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool useGrid;

void main() {
    // Ambient
    float ambientStrength = 0.35;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.4;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 baseColor = objectColor;

    // Optional grid pattern for platform floor surface
    if (useGrid) {
        vec2 coord = FragPos.xz;
        vec2 grid = abs(fract(coord - 0.5) - 0.5) / (fwidth(coord) + 0.001);
        float line = min(grid.x, grid.y);
        float c = 1.0 - min(line, 1.0);
        baseColor = mix(objectColor, vec3(0.15, 0.18, 0.22), c * 0.7);
    }

    vec3 result = (ambient + diffuse + specular) * baseColor;
    FragColor = vec4(result, 1.0);
}
