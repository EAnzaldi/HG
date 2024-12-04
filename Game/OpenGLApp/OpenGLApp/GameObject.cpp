#include "GameObject.h"

#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject(glm::vec2 position, glm::vec2 size, TextureObj texture)
    : Position(position), Size(size), Rotation(0.0f), Texture(texture)
{
    this->initRenderData();
}

// Inizializzo i dati nei buffer
void GameObject::initRenderData() {

    float vertices[] = {
        // Posizioni              // Colore                 // Coordinate texture
        -0.5f, 0.5f, 0.0f,        0.0f, 1.0f, 0.0f,         0.0f, 1.0f, // Vertice in alto a sinistra
        0.5f, 0.5f, 0.0f,         0.0f, 1.0f, 0.0f,         1.0f, 1.0f, // Vertice in alto a destra
        0.5f, -0.5f, 0.0f,        0.0f, 1.0f, 0.0f,         1.0f, 0.0f, // Vertice in basso a destra
        -0.5f, -0.5f, 0.0f,       0.0f, 1.0f, 0.0f,         0.0f, 0.0f  // Vertice in basso a sinistra
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, EBO;
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void GameObject::Render(const Shader& shader) const {
    shader.use();

    // Modifico model altrimenti disegnerei un quadrato in posizione 0,0
    glm::mat4 model = glm::mat4(1.0f);

    // Posizione
    model = glm::translate(model, glm::vec3(this->Position, 0.0f));

    // Scala
    model = glm::scale(model, glm::vec3(this->Size, 1.0f));

    shader.setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->Texture.TextureID);

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Hitbox GameObject::GetHitbox() const {
    return Hitbox{
        this->Position - this->Size / 2.0f, // Min (basso a sinistra)
        this->Position + this->Size / 2.0f  // Max (alto a destra)
    };
}

bool GameObject::CheckCollision(const GameObject& other) const {
    Hitbox bounds1 = this->GetHitbox();
    Hitbox bounds2 = other.GetHitbox();
    return (bounds1.Min.x < bounds2.Max.x && bounds1.Max.x > bounds2.Min.x &&
        bounds1.Min.y < bounds2.Max.y && bounds1.Max.y > bounds2.Min.y);
}