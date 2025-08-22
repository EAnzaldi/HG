#include "Enemy.h"
#include <glm/glm.hpp>  // Per il calcolo dei vettori

Enemy::Enemy(glm::vec2 position, glm::vec3 size, Model* model, TextureObject* texture, bool repeatWidth, glm::vec2 velocity, bool moveRight)
    : MovingObject(position, size, model, texture, repeatWidth, velocity, moveRight)
{

}
Enemy::Enemy(glm::vec2 position, glm::vec3 size, FlatMesh* fmesh, bool repeatWidth, glm::vec2 velocity, bool moveRight)
    : MovingObject(position, size, fmesh, repeatWidth, velocity, moveRight)
{

}
void Enemy::HandleCollisionWithSolid(GameObject* solidObject)
{
    Hitbox thisHitbox = this->GetHitbox();
    Hitbox solidHitbox = solidObject->GetHitbox();

    float overlapX = std::min(thisHitbox.Max.x, solidHitbox.Max.x) - std::max(thisHitbox.Min.x, solidHitbox.Min.x);
    float overlapY = std::min(thisHitbox.Max.y, solidHitbox.Max.y) - std::max(thisHitbox.Min.y, solidHitbox.Min.y);

    // Correggo l'overlap minore
    if (overlapX < overlapY) // Correggo sull'asse X
    {
        if (this->Position.x < solidObject->Position.x) // Collisione a destra del MovingObject
        {
            this->Position.x -= overlapX;
            this->velocity.x = -this->velocity.x;
        }
        else // Collisione a sinistra del MovingObject
        {
            this->Position.x += overlapX;
            this->velocity.x = -this->velocity.x;
        }
    }
    else // Correggi sull'asse Y
    {
        if (this->Position.y < solidObject->Position.y) // Collisione sopra del MovingObject
        {
            this->Position.y -= overlapY;
        }
        else // Collisione sotto del MovingObject
        {
            this->Position.y += overlapY;
            this->isOnGround = true;
        }
        this->velocity.y = 0;
    }
}