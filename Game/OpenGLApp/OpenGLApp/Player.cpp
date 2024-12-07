#include "Player.h"

Player::Player(glm::vec2 position, glm::vec2 size, TextureObj texture, bool repeatWidth)
    : GameObject(position, size, texture, repeatWidth), Velocity(0.0f, 0.0f) {
}

void Player::Move(float deltaTime) {
    this->Position += this->Velocity * deltaTime;
}

void Player::CheckCollision(const std::vector<GameObject>& solidObjects)
{
    for (const GameObject& solid : solidObjects)
    {
        if (IsCollided(solid))
        {
            HandleCollision(solid);
        }
    }
}

bool DEAD = false;//Player inizia vivo

bool Player::CheckCollision(const Enemy& enemy)
{
    if (IsCollided(enemy))
    {
        HandleCollision(enemy);
        std::cout << "GAME OVER" << std::endl;
        DEAD = true;//Player muore
    }
    return DEAD;
}

bool Player::IsCollided(const GameObject& other) const {
    Hitbox bounds1 = this->GetHitbox();
    Hitbox bounds2 = other.GetHitbox();
    return (bounds1.Min.x < bounds2.Max.x && bounds1.Max.x > bounds2.Min.x &&
        bounds1.Min.y < bounds2.Max.y && bounds1.Max.y > bounds2.Min.y);
}

void Player::HandleCollision(const GameObject& solid)
{
    // Blocco movimento verticale
    if (this->Velocity.y > 0)  // Collisione sopra
    {
        this->Position.y = solid.GetHitbox().Min.y - this->Size.y / 2.0f;
    }
    else if (this->Velocity.y < 0)  // Collisione sotto
    {
        this->Position.y = solid.GetHitbox().Max.y + this->Size.y / 2.0f;
    }

    // Blocco movimento orizzontale
    if (this->Velocity.x > 0)  // Collisione a destra
    {
        this->Position.x = solid.GetHitbox().Min.x - this->Size.x / 2.0f;
    }
    else if (this->Velocity.x < 0)  // Collisione a sinistra
    {
        this->Position.x = solid.GetHitbox().Max.x + this->Size.x / 2.0f;
    }

}