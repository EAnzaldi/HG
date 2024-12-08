#include "Player.h"

Player::Player(glm::vec2 position, glm::vec2 size, TextureObject texture, bool repeatWidth)
    : GameObject(position, size, texture, repeatWidth), Velocity(0.0f, 0.0f) {
}

void Player::Move(float deltaTime) 
{
    if (isMidAir) 
    {
        this->Velocity.y += gravityForce * deltaTime;
    }

    this->Position += this->Velocity * deltaTime;
}

void Player::HandleJump(float deltaTime)
{
    if (!this->isMidAir)
    {
        this->isMidAir = true;
        this->Velocity.y = this->baseJumpForce;
        this->currentJumpTime = 0.0f;
    }
    else if (!this->isPastJumpPeak)
    {
        this->currentJumpTime += deltaTime;
        if (this->currentJumpTime < this->maxJumpTime)
        {
            this->Velocity.y += this->addedJumpForce * deltaTime;
        }
        else
        {
            this->isPastJumpPeak = true;
        }
    }
}

void Player::CheckCollision(const std::vector<GameObject>& solidObjects)
{
    isOnGround = false;

    for (const GameObject& solid : solidObjects)
    {
        if (HasCollided(solid))
        {
            HandleCollision(solid);
        }
    }

    if (!isOnGround)
    {
        this->isMidAir = true;
    }
    else
    {
        this->isMidAir = false;
    }
}

bool Player::CheckCollision(const Enemy& enemy)
{
    if (HasCollided(enemy))
    {
        std::cout << "GAME OVER" << std::endl;
        isDead = true;  // Player muore
    }
    return isDead;
}

bool Player::HasCollided(const GameObject& other) const {
    Hitbox bounds1 = this->GetHitbox();
    Hitbox bounds2 = other.GetHitbox();
    return (bounds1.Min.x <= bounds2.Max.x && bounds1.Max.x >= bounds2.Min.x &&
        bounds1.Min.y <= bounds2.Max.y && bounds1.Max.y >= bounds2.Min.y);
}

void Player::HandleCollision(const GameObject& solid)
{
    this->isOnGround = false;

    Hitbox playerHitbox = this->GetHitbox();
    Hitbox solidHitbox = solid.GetHitbox();

    // Calcolo la compenetrazione per ogni asse
    float overlapX = std::min(playerHitbox.Max.x, solidHitbox.Max.x) - std::max(playerHitbox.Min.x, solidHitbox.Min.x);
    float overlapY = std::min(playerHitbox.Max.y, solidHitbox.Max.y) - std::max(playerHitbox.Min.y, solidHitbox.Min.y);

    // Correggo l'overlap minore
    if (overlapX < overlapY) // Correggo sull'asse X
    {
        if (this->Position.x < solid.Position.x) // Collisione a destra del player
        {
            this->Position.x -= overlapX;
        }
        else // Collisione a sinistra del player
        {
            this->Position.x += overlapX; // Sposta il player verso destra
        }
        this->Velocity.x = 0;
    }
    else // Correggi sull'asse Y
    {
        if (this->Position.y < solid.Position.y) // Collisione sopra il player
        {
            this->Position.y -= overlapY;
        }
        else // Collisione sotto il player
        {
            this->Position.y += overlapY; 
            this->isOnGround = true;
            this->isPastJumpPeak = false;
        }
        this->Velocity.y = 0;
    }
}