#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "renderer/Shader.h"
#include "renderer/Mesh.h"
#include "core/Camera.h"
#include "renderer/Material.h"
#include "renderer/ShadowMap.h"

class Renderer {
public:
    glm::vec3 lightPos;
    glm::vec3 lightColor;
    ShadowMap shadowMap;

    Renderer();

    void init();
    void clear(float r = 0.08f, float g = 0.09f, float b = 0.12f, float a = 1.0f) const;

    // Shadow Pass Functions
    glm::mat4 getLightSpaceMatrix() const;
    void beginShadowPass(Shader& shadowShader);
    void renderMeshToShadowMap(const Mesh& mesh, Shader& shadowShader, const glm::mat4& modelMatrix) const;
    void endShadowPass(int windowWidth, int windowHeight) const;

    // Main Pass Function
    void renderMeshWithShadows(
        const Mesh& mesh,
        Shader& mainShader,
        const Camera& camera,
        float aspectRatio,
        const glm::mat4& modelMatrix,
        const Material& material,
        bool useGrid = false
    ) const;
};

#endif
