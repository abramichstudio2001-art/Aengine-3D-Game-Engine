#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    Material(
        glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f),
        glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f),
        float shininess = 32.0f
    ) : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

    static Material Emerald() {
        return Material(glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.07568f, 0.61424f, 0.07568f), glm::vec3(0.633f, 0.727811f, 0.633f), 76.8f);
    }

    static Material Gold() {
        return Material(glm::vec3(0.24725f, 0.1995f, 0.0745f), glm::vec3(0.75164f, 0.60648f, 0.22648f), glm::vec3(0.628281f, 0.555802f, 0.366065f), 51.2f);
    }

    static Material Bronze() {
        return Material(glm::vec3(0.2125f, 0.1275f, 0.054f), glm::vec3(0.714f, 0.4284f, 0.18144f), glm::vec3(0.393548f, 0.271906f, 0.166721f), 25.6f);
    }

    static Material SlatePlatform() {
        return Material(glm::vec3(0.15f, 0.18f, 0.22f), glm::vec3(0.45f, 0.52f, 0.60f), glm::vec3(0.3f, 0.3f, 0.35f), 16.0f);
    }

    static Material VibrantOrange() {
        return Material(glm::vec3(0.25f, 0.1f, 0.05f), glm::vec3(0.95f, 0.45f, 0.15f), glm::vec3(0.8f, 0.8f, 0.8f), 64.0f);
    }
};

#endif
