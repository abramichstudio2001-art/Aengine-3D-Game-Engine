#include "renderer/Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure &e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
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
