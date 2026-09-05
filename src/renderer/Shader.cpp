#include "renderer/Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

// Embedded fallback GLSL shader code for standalone execution
static const char* FALLBACK_DEFAULT_VERT = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

static const char* FALLBACK_DEFAULT_FRAG = R"(#version 330 core
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

    float up = dot(norm, vec3(0.0, 1.0, 0.0)) * 0.5 + 0.5;
    vec3 skyColor = vec3(0.25, 0.32, 0.45) * 1.2;
    vec3 groundColor = vec3(0.08, 0.07, 0.06);
    vec3 ambient = mix(groundColor, skyColor, up) * material.ambient * lightColor * 1.3;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse * lightColor * 1.4;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * material.specular * lightColor * 1.2;

    float rim = 1.0 - max(dot(viewDir, norm), 0.0);
    rim = pow(rim, 3.0);
    vec3 rimColor = vec3(0.4, 0.5, 0.7) * rim * diff * 0.35;

    if (useGrid) {
        vec2 coord = FragPos.xz * 1.0;
        vec2 grid = abs(fract(coord - 0.5) - 0.5) / (fwidth(coord) + 0.0001);
        float line = min(grid.x, grid.y);
        float gridPattern = 1.0 - clamp(line, 0.0, 1.0);

        vec3 gridLineColor = vec3(0.20, 0.28, 0.42) * 1.5;
        vec3 baseDiffuse = mix(material.diffuse, gridLineColor, gridPattern * 0.85);
        diffuse = diff * baseDiffuse * lightColor * 1.4;
    }

    float shadow = ShadowCalculation(FragPosLightSpace, norm, lightDir);
    vec3 result = ambient + (1.0 - shadow) * (diffuse + specular) + rimColor;

    result = ACESFilm(result);
    result = pow(result, vec3(1.0 / 2.2));

    FragColor = vec4(result, 1.0);
}
)";

static const char* FALLBACK_SHADOW_VERT = R"(#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main() {
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}
)";

static const char* FALLBACK_SHADOW_FRAG = R"(#version 330 core

void main() {
    // Depth is written automatically
}
)";

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;

    // Attempt reading from primary or fallback file paths without iostream exceptions
    std::ifstream vShaderFile(vertexPath);
    std::ifstream fShaderFile(fragmentPath);

    if (!vShaderFile.is_open() || !fShaderFile.is_open()) {
        std::string fallbackVPath = std::string("../") + vertexPath;
        std::string fallbackFPath = std::string("../") + fragmentPath;
        vShaderFile.clear();
        fShaderFile.clear();
        vShaderFile.open(fallbackVPath);
        fShaderFile.open(fallbackFPath);
    }

    if (vShaderFile.is_open() && fShaderFile.is_open()) {
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }

    // If external files are missing or empty, use embedded fallback shaders
    if (vertexCode.empty() || fragmentCode.empty()) {
        std::string vPathStr(vertexPath);
        if (vPathStr.find("shadow") != std::string::npos) {
            std::cout << "[Shader] Using embedded fallback shadow shaders." << std::endl;
            vertexCode = FALLBACK_SHADOW_VERT;
            fragmentCode = FALLBACK_SHADOW_FRAG;
        } else {
            std::cout << "[Shader] Using embedded fallback default shaders." << std::endl;
            vertexCode = FALLBACK_DEFAULT_VERT;
            fragmentCode = FALLBACK_DEFAULT_FRAG;
        }
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    if (ID != 0) {
        glDeleteProgram(ID);
    }
}

void Shader::use() const {
    glUseProgram(ID);
}

GLint Shader::getUniformLocation(const std::string &name) {
    auto it = uniformLocationCache.find(name);
    if (it != uniformLocationCache.end()) {
        return it->second;
    }
    GLint location = glGetUniformLocation(ID, name.c_str());
    uniformLocationCache[name] = location;
    return location;
}

void Shader::setBool(const std::string &name, bool value) {
    glUniform1i(getUniformLocation(name), (int)value);
}

void Shader::setInt(const std::string &name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string &name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) {
    glUniform2fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) {
    glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) {
    glUniform4fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMaterial(const std::string &name, const Material &material) {
    setVec3(name + ".ambient", material.ambient);
    setVec3(name + ".diffuse", material.diffuse);
    setVec3(name + ".specular", material.specular);
    setFloat(name + ".shininess", material.shininess);
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
