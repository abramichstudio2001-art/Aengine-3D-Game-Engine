#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "renderer/Shader.h"
#include "renderer/Mesh.h"
#include "core/Camera.h"

class Renderer {
public:
    glm::vec3 lightPos;
    glm::vec3 lightColor;

    Renderer();

    void init();
    void clear(float r = 0.08f, float g = 0.09f, float b = 0.12f, float a = 1.0f) const;
    void renderMesh(const Mesh& mesh, Shader& shader, const Camera& camera, float aspectRatio, const glm::mat4& modelMatrix, const glm::vec3& color, bool useGrid = false) const;
};

#endif
