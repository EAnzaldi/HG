#include "Enemy.h"
#include <glm/glm.hpp>  // Per il calcolo dei vettori

Enemy::Enemy(glm::vec2 position, glm::vec3 size, Model* model, bool repeatWidth, glm::vec2 velocity, bool moveRight)
    : MovingObject(position, size, model, repeatWidth, velocity, moveRight)
{

}
Enemy::Enemy(glm::vec2 position, glm::vec3 size, TextureObject* texture, bool repeatWidth, glm::vec2 velocity, bool moveRight)
    : MovingObject(position, size, texture, repeatWidth, velocity, moveRight)
{

}
float Enemy::speedBonus = 1.0f;
void Enemy::HandleCollisionWithSolid(GameObject* solidObject, Collision collision)
{
    /*
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
            this->velocity.x = -this->velocity.x;//rimbalza il nemico che cambia direzione
        }
        else // Collisione a sinistra del MovingObject
        {
            this->Position.x += overlapX;
            this->velocity.x = -this->velocity.x;//rimbalza il nemico che cambia direzione
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
    }*/

    MovingObject::HandleCollisionWithSolid(solidObject, collision);

    if (collision == Collision::Left || collision == Collision::Right) {
        this->velocity.x = -this->velocity.x;//rimbalza il nemico che cambia direzione
    }
}
void Enemy::Move(float deltaTime)
{
    float original = velocity.x;
    velocity.x *= speedBonus;
    MovingObject::Move(deltaTime);
    Flip();
    velocity.x = original;
}
void Enemy::Flip() {
    // Ribaltamento del modello per seguire la direzione del nemico
    if (velocity.x > 0.0f && !lastDirectionRight) {
        lastDirectionRight = true;
    }
    else if (velocity.x < 0.0f && lastDirectionRight) {
        lastDirectionRight = false;
    }

    FlipX = lastDirectionRight ? 1.0f : -1.0f;
}
void Enemy::SpeedUp(float amount)
{
    speedBonus *= amount;
}
void Enemy::SpeedDown(float amount)
{
    speedBonus /= amount;
}