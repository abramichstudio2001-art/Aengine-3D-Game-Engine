#include "renderer/Renderer.h"
#include <glad/glad.h>

Renderer::Renderer()
    : lightPos(glm::vec3(6.5f, 11.0f, 5.5f)),
      lightColor(glm::vec3(1.0f, 0.97f, 0.88f)),
      shadowMap(2048, 2048) {
}

void Renderer::init() {
    shadowMap.init();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);
}

void Renderer::clear(float r, float g, float b, float a) const {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

glm::mat4 Renderer::getLightSpaceMatrix() const {
    return shadowMap.getLightSpaceMatrix(lightPos, glm::vec3(0.0f, 0.0f, 0.0f));
}

void Renderer::beginShadowPass(Shader& shadowShader) {
    shadowMap.bindForWriting();
    shadowShader.use();
    shadowShader.setMat4("lightSpaceMatrix", getLightSpaceMatrix());
}

void Renderer::renderMeshToShadowMap(const Mesh& mesh, Shader& shadowShader, const glm::mat4& modelMatrix) const {
    shadowShader.setMat4("model", modelMatrix);
    mesh.draw();
}

void Renderer::endShadowPass(int windowWidth, int windowHeight) const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowWidth, windowHeight);
}

void Renderer::renderMeshWithShadows(
    const Mesh& mesh,
    Shader& mainShader,
    const Camera& camera,
    float aspectRatio,
    const glm::mat4& modelMatrix,
    const Material& material,
    bool useGrid
) const {
    mainShader.use();

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix(aspectRatio);

    mainShader.setMat4("model", modelMatrix);
    mainShader.setMat4("view", view);
    mainShader.setMat4("projection", projection);
    mainShader.setMat4("lightSpaceMatrix", getLightSpaceMatrix());

    mainShader.setMaterial("material", material);
    mainShader.setVec3("lightColor", lightColor);
    mainShader.setVec3("lightPos", lightPos);
    mainShader.setVec3("viewPos", camera.Position);
    mainShader.setBool("useGrid", useGrid);

    // Bind depth shadow map texture to unit 1
    shadowMap.bindForReading(GL_TEXTURE1);
    mainShader.setInt("shadowMap", 1);

    mesh.draw();
}
