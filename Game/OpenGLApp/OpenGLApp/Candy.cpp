#include "Candy.h"

Candy::Candy(glm::vec2 position, glm::vec3 size, FlatMesh* fmesh, bool repeatWidth)
    : MovingObject(position, size, fmesh, repeatWidth, velocity = glm::vec2(0.0f, 0.0f), false) {
}

void Candy::Move(float deltaTime)
{
    if (!isOnGround)
    {
        this->velocity.y += gravityForce * deltaTime;
    }

    this->Position += this->velocity * deltaTime;
}
void Candy::Render(const Shader& Shader) const
{
    GameObject::Render(Shader);
}