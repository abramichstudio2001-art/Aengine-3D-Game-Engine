#include "renderer/Renderer.h"

Renderer::Renderer()
    : lightPos(glm::vec3(3.0f, 6.0f, 4.0f)), lightColor(glm::vec3(1.0f, 1.0f, 0.95f)) {
}

void Renderer::init() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void Renderer::clear(float r, float g, float b, float a) const {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::renderMesh(const Mesh& mesh, Shader& shader, const Camera& camera, float aspectRatio, const glm::mat4& modelMatrix, const glm::vec3& color, bool useGrid) const {
    shader.use();

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix(aspectRatio);

    shader.setMat4("model", modelMatrix);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    shader.setVec3("objectColor", color);
    shader.setVec3("lightColor", lightColor);
    shader.setVec3("lightPos", lightPos);
    shader.setVec3("viewPos", camera.Position);
    shader.setBool("useGrid", useGrid);

    mesh.draw();
}
