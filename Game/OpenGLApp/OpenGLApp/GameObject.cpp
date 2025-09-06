#include "GameObject.h"

FlatMesh* GameObject::fmesh = nullptr;

GameObject::GameObject(glm::vec2 position, glm::vec3 size, Model* model, bool repeatWidth)
    : Position(position), Size(size), Rotation(0.0f), model(model), Texture(nullptr), RepeatWidth(repeatWidth), FlipX(1.0f), Dimension(DimensionType::ThreeD)
{
    // flag specifica se si voglia scalare la texture (consigliato=1 per piattaforme)
}

GameObject::GameObject(glm::vec2 position, glm::vec3 size, TextureObject* texture, bool repeatWidth)
    : Position(position), Size(size), Rotation(0.0f), model(nullptr), Texture(texture), RepeatWidth(repeatWidth), FlipX(1.0f), Dimension(DimensionType::TwoD)
{
    if (!fmesh) fmesh = new FlatMesh();
}

void GameObject::Render(const Shader& shader) const
{
    shader.use();

    // Modifico model altrimenti disegnerei un quadrato in posizione 0,0
    glm::mat4 model_mat = glm::mat4(1.0f);

    // Posizione
    model_mat = glm::translate(model_mat, glm::vec3(this->Position, 0.0f));

    // Rotazione
    model_mat = glm::rotate(model_mat, glm::radians(this->Rotation), glm::vec3(0.0f, 1.0f, 0.0f)); // Ruota attorno all'asse Z

    // Scalamento
    //model_mat = glm::scale(model_mat, this->Size);
    model_mat = glm::scale(model_mat, glm::vec3(FlipX * this->Size.x, this->Size.y, 1.0f));

    Render(shader, model_mat);
}
void GameObject::Render(const Shader& shader, const glm::mat4 model_mat) const {
    shader.setMat4("model", model_mat);

    if (Dimension == DimensionType::ThreeD)
        Render3D(shader);
    else if (Dimension == DimensionType::TwoD)
        Render2D(shader);
}

void GameObject::Render3D(const Shader& shader) const 
{
    model->Draw(shader);
}

void GameObject::Render2D(const Shader& shader) const
{
    if (RepeatWidth)
        shader.setVec2("textureReps", glm::vec2(Size.x * 10.0f, 1.0f));
    else
        shader.setVec2("textureReps", glm::vec2(1.0f, 1.0f));

    fmesh->Draw(shader, Texture->TextureID);
}
Hitbox GameObject::GetHitbox() const
{
    if (Dimension == DimensionType::ThreeD)
        return GetHitbox3D();
    else if(Dimension == DimensionType::TwoD)
        return GetHitbox2D();
}
Hitbox GameObject::GetHitbox3D() const
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
Hitbox GameObject::GetHitbox2D() const
{
    glm::vec2 size = glm::vec2(Size.x, Size.y);
    return Hitbox{ Position - size / 2.0f, Position + size / 2.0f };
}
void GameObject::Print() const
{
    printf("Position:%0.1f,%0.1f ", Position.x, Position.y);
    printf("Size:%0.1f,%0.1f,%0.1f ", Size.x, Size.y, Size.z);
    printf("Rotation:%0.1f ", Rotation);
    printf("Size:0.1f ", Size);
    printf("FlipX:0.1f ", FlipX);
    printf("RepeatWidth:%d ", RepeatWidth);
    if (Dimension == DimensionType::ThreeD)
        printf("3D\n");
    else if (Dimension == DimensionType::TwoD)
        printf("2D\n");
}
//Distanza euclidea da *pObj
float GameObject::DistanceTo(GameObject* pObj) const
{
    float x2 = (Position.x - pObj->Position.x) * (Position.x - pObj->Position.x);
    float y2 = (Position.y - pObj->Position.y) * (Position.y - pObj->Position.y);
    return sqrt(x2 + y2);
}