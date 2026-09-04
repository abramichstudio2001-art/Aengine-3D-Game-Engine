#include "renderer/Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : vertices(vertices), indices(indices), VAO(0), VBO(0), EBO(0) {
    setupMesh();
}

Mesh::~Mesh() {
    cleanup();
}

Mesh::Mesh(Mesh&& other) noexcept
    : vertices(std::move(other.vertices)),
      indices(std::move(other.indices)),
      VAO(other.VAO),
      VBO(other.VBO),
      EBO(other.EBO) {
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        cleanup();
        vertices = std::move(other.vertices);
        indices = std::move(other.indices);
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
    }
    return *this;
}

void Mesh::cleanup() {
    if (EBO != 0) glDeleteBuffers(1, &EBO);
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
    VAO = VBO = EBO = 0;
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // Texture Coords attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Mesh::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Mesh Mesh::createPlatform(float width, float depth, float height) {
    float hw = width * 0.5f;
    float hd = depth * 0.5f;
    float hh = height * 0.5f;

    std::vector<Vertex> verts = {
        // Top Face (y = hh), CCW from above
        {{-hw,  hh,  hd}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ hw,  hh,  hd}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{ hw,  hh, -hd}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-hw,  hh, -hd}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},

        // Bottom Face (y = -hh), CCW from below
        {{-hw, -hh, -hd}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ hw, -hh, -hd}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        {{ hw, -hh,  hd}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-hw, -hh,  hd}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},

        // Front Face (z = hd), CCW from front
        {{-hw, -hh,  hd}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{ hw, -hh,  hd}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{ hw,  hh,  hd}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-hw,  hh,  hd}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

        // Back Face (z = -hd), CCW from back
        {{ hw, -hh, -hd}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{-hw, -hh, -hd}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
        {{-hw,  hh, -hd}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{ hw,  hh, -hd}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},

        // Left Face (x = -hw), CCW from left
        {{-hw, -hh, -hd}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{-hw, -hh,  hd}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-hw,  hh,  hd}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-hw,  hh, -hd}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

        // Right Face (x = hw), CCW from right
        {{ hw, -hh,  hd}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{ hw, -hh, -hd}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{ hw,  hh, -hd}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{ hw,  hh,  hd}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}
    };

    std::vector<unsigned int> inds;
    for (int i = 0; i < 6; ++i) {
        unsigned int offset = i * 4;
        inds.push_back(offset + 0);
        inds.push_back(offset + 1);
        inds.push_back(offset + 2);
        inds.push_back(offset + 0);
        inds.push_back(offset + 2);
        inds.push_back(offset + 3);
    }

    return Mesh(verts, inds);
}

Mesh Mesh::createCube(float size) {
    return createPlatform(size, size, size);
}
