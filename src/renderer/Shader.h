#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include "renderer/Material.h"

class Shader {
public:
    GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use() const;

    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setVec2(const std::string &name, const glm::vec2 &value);
    void setVec3(const std::string &name, const glm::vec3 &value);
    void setVec4(const std::string &name, const glm::vec4 &value);
    void setMat4(const std::string &name, const glm::mat4 &mat);
    void setMaterial(const std::string &name, const Material &material);

private:
    std::unordered_map<std::string, GLint> uniformLocationCache;
    GLint getUniformLocation(const std::string &name);
    void checkCompileErrors(GLuint shader, const std::string& type);
};

#endif
