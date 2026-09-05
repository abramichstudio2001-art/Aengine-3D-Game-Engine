#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ShadowMap {
public:
    GLuint depthMapFBO;
    GLuint depthMap;
    unsigned int width;
    unsigned int height;

    ShadowMap(unsigned int width = 2048, unsigned int height = 2048);
    ~ShadowMap();

    void init();
    void bindForWriting() const;
    void bindForReading(GLenum textureUnit) const;
    glm::mat4 getLightSpaceMatrix(const glm::vec3& lightPos, const glm::vec3& targetPos = glm::vec3(0.0f)) const;
};

#endif
