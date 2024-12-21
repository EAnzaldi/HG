#include "Player.h"

Player::Player(glm::vec2 position, glm::vec3 size, Model model, TextureObject* texture, bool repeatWidth)
    : MovingObject(position, size, model, texture, repeatWidth, velocity = glm::vec2(0.0f, 0.0f)),
    invincibilityDuration(1.0f), invincibilityTimer(0.0f), isInvincible(false) {
}

void Player::HandleJump(float deltaTime, irrklang::ISoundEngine* engine)
{
    if (this->isOnGround)
    {
        this->isOnGround = false;
        this->velocity.y = this->baseJumpForce;
        this->currentJumpTime = 0.0f;
        engine->play2D("resources/sounds/jump.wav");
    }
    else if (!this->isPastJumpPeak)
    {
        this->currentJumpTime += deltaTime;
        if (this->currentJumpTime < this->maxJumpTime)
        {
            this->velocity.y += this->addedJumpForce * deltaTime;
        }
        else
        {
            this->isPastJumpPeak = true;
        }
    }
}

bool Player::CheckEnemyCollision(const Enemy& enemy)
{
    if (!isInvincible && CheckCollision(enemy))
    {
        if (lives > 1)
        {
            lives--; // Perde una vita
            std::cout << "Lives left: " << lives << std::endl;
            StartInvincibility(); // Inizia l'invincibilità
        }
        else
        {
            std::cout << "GAME OVER" << std::endl;
            isDead = true;
        }
    }
    return isDead;
}

void Player::HandleCollisionWithSolid(GameObject solidObject)
{
    Hitbox playerHitbox = this->GetHitbox();
    Hitbox solidHitbox = solidObject.GetHitbox();

    float overlapX = std::min(playerHitbox.Max.x, solidHitbox.Max.x) - std::max(playerHitbox.Min.x, solidHitbox.Min.x);
    float overlapY = std::min(playerHitbox.Max.y, solidHitbox.Max.y) - std::max(playerHitbox.Min.y, solidHitbox.Min.y);

    // Correggo l'overlap minore
    if (overlapX < overlapY) // Correggo sull'asse X
    {
        if (this->Position.x < solidObject.Position.x) // Collisione a destra del player
        {
            this->Position.x -= overlapX;
        }
        else // Collisione a sinistra del player
        {
            this->Position.x += overlapX; // Sposta il player verso destra
        }
        this->velocity.x = 0;
    }
    else // Correggi sull'asse Y
    {
        if (this->Position.y < solidObject.Position.y) // Collisione sopra il player
        {
            this->Position.y -= overlapY;
        }
        else // Collisione sotto il player
        {
            this->Position.y += overlapY;
            this->isOnGround = true;
            this->isPastJumpPeak = false;
        }
        this->velocity.y = 0;
    }
}

void Player::Update(float deltaTime)
{
    // Aggiorna il timer di invincibilità
    if (isInvincible)
    {
        invincibilityTimer += deltaTime;
        if (invincibilityTimer >= invincibilityDuration)
        {
            isInvincible = false;  // Fine invincibilità
            invincibilityTimer = 0.0f;
            std::cout << "Invincibility ended" << std::endl;
        }
    }

    // Altri aggiornamenti del giocatore
}

void Player::StartInvincibility()
{
    isInvincible = true;
    invincibilityTimer = 0.0f; // Resetta il timer
}