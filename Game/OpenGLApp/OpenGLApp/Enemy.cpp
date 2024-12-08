#include "Enemy.h"

Enemy::Enemy(glm::vec2 position, glm::vec2 size, TextureObject texture, bool repeatWidth)
    : GameObject(position, size, texture, repeatWidth), Velocity(0.0f, 0.0f) {
}

void Enemy::Move(float deltaTime) 
{
    this->Position += this->Velocity * deltaTime;
}
