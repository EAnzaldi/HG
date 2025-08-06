#include "GameObject.h"

#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject(glm::vec2 position, glm::vec3 size, Model* model, TextureObject* texture, bool repeatWidth)
    : Position(position), Size(size), Rotation(0.0f), model(model), Texture(texture), fmesh(nullptr), RepeatWidth(repeatWidth), FlipX(1.0f)
{
    // flag specifica se si voglia scalare la texture (consigliato=1 per piattaforme)
}

GameObject::GameObject(glm::vec2 position, glm::vec3 size, FlatMesh* fmesh, bool repeatWidth)
    : Position(position), Size(size), Rotation(0.0f), model(nullptr), Texture(nullptr), fmesh(fmesh), RepeatWidth(repeatWidth), FlipX(1.0f)
{
    // flag specifica se si voglia scalare la texture (consigliato=1 per piattaforme)
}

void GameObject::Render(const Shader& shader) const 
{
    shader.use();

    if (this->Texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->Texture->TextureID);
        if(RepeatWidth)
            shader.setVec2("textureReps", glm::vec2(Size.x * 10.0f, 1.0f));
        else
            shader.setVec2("textureReps", glm::vec2(1.0f, 1.0f));
    }

    // Modifico model altrimenti disegnerei un quadrato in posizione 0,0
    glm::mat4 model_mat = glm::mat4(1.0f);

    // Posizione
    model_mat = glm::translate(model_mat, glm::vec3(this->Position, 0.0f));

    // Rotazione
    model_mat = glm::rotate(model_mat, glm::radians(this->Rotation), glm::vec3(0.0f, 1.0f, 0.0f)); // Ruota attorno all'asse Z

    // Scalamento
    //model_mat = glm::scale(model_mat, this->Size);
    model_mat = glm::scale(model_mat, glm::vec3(FlipX * this->Size.x, this->Size.y, 1.0f));


    shader.setMat4("model", model_mat);

    model->Draw(shader);
}

void GameObject::RenderFlat(const Shader& shader) const
{
    shader.use();

    if (RepeatWidth)
        shader.setVec2("textureReps", glm::vec2(Size.x * 10.0f, 1.0f));
    else
        shader.setVec2("textureReps", glm::vec2(1.0f, 1.0f));

    // Modifico model altrimenti disegnerei un quadrato in posizione 0,0
    glm::mat4 model_mat = glm::mat4(1.0f);

    // Posizione
    model_mat = glm::translate(model_mat, glm::vec3(this->Position, 0.0f));

    // Rotazione
    model_mat = glm::rotate(model_mat, glm::radians(this->Rotation), glm::vec3(0.0f, 1.0f, 0.0f)); // Ruota attorno all'asse Z

    // Scalamento
    //model_mat = glm::scale(model_mat, this->Size);
    model_mat = glm::scale(model_mat, glm::vec3(FlipX * this->Size.x, this->Size.y, 1.0f));

    shader.setMat4("model", model_mat);

    fmesh->Draw(shader);
}

Hitbox GameObject::GetHitbox() const
{
    glm::vec3 min_point = glm::vec3(FLT_MAX);  // Inizializza a un valore molto grande
    glm::vec3 max_point = glm::vec3(-FLT_MAX); // Inizializza a un valore molto piccolo

    // Itera su tutte le mesh del modello
    for (const auto& mesh : model->meshes)
    {
        // Itera su tutti i vertici della mesh
        for (const auto& vertex : mesh.vertices)
        {
            // Confronta le coordinate x, y e z per determinare i minimi e massimi
            min_point.x = std::min(min_point.x, vertex.Position.x);
            min_point.y = std::min(min_point.y, vertex.Position.y);

            max_point.x = std::max(max_point.x, vertex.Position.x);
            max_point.y = std::max(max_point.y, vertex.Position.y);
        }
    }

    // Applica la scala al calcolo della hitbox
    min_point.x *= this->Size.x;
    min_point.y *= this->Size.y;
    max_point.x *= this->Size.x;
    max_point.y *= this->Size.y;

    // Considera la posizione nel calcolo della hitbox
    min_point.x += this->Position.x;
    min_point.y += this->Position.y;
    max_point.x += this->Position.x;
    max_point.y += this->Position.y;

    // Calcola il centro della bounding box
    glm::vec2 center = glm::vec2((min_point.x + max_point.x) / 2.0f, (min_point.y + max_point.y) / 2.0f);

    // Calcola le dimensioni della bounding box
    glm::vec2 size = glm::vec2(max_point.x - min_point.x, max_point.y - min_point.y);

    return Hitbox{ center - size / 2.0f, center + size / 2.0f }; // Restituisci la hitbox
}
