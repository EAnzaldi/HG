#include "Enemy.h"
#include <glm/glm.hpp>  // Per il calcolo dei vettori

Enemy::Enemy(glm::vec2 position, glm::vec2 size, TextureObject texture, bool repeatWidth)
    : GameObject(position, size, texture, repeatWidth), Velocity(0.0f, 0.0f), Destination(position) {
}

void Enemy::SetDestination(glm::vec2 destination) {
    this->Destination = destination;

    // Calcolo della direzione normalizzata
    glm::vec2 direction = this->Destination - this->Position;
    if (glm::length(direction) > 0.0f) {
        this->Velocity = glm::normalize(direction);  // Direzione unitaria
    }
}

void Enemy::Move(float deltaTime)
{
    if (isMidAir)
    {
        this->Velocity.y += gravityForce * deltaTime;
    }

    this->Position.x += (this->Velocity.x * deltaTime)/4;   //in player viene usato solo this->Position mentre qui voglio che la sua x
    this->Position.y += this->Velocity.y * deltaTime;       //sia più lenta della y che deve rimanere fissa essendo la forza di gravità

    // Limita il movimento orizzontale ai bordi dello schermo (opzionale)
    if (this->Position.x > 1.0f - this->Size.x / 2) {
        this->Velocity.x = -std::abs(this->Velocity.x); // Inverti direzione orizzontale
    }
    if (this->Position.x < -1.0f + this->Size.x / 2) {
        this->Velocity.x = std::abs(this->Velocity.x); // Inverti direzione orizzontale
    }
}

void Enemy::HandleJump(float deltaTime)
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

bool Enemy::HasReachedDestination() const {
    float distance = glm::length(this->Destination - this->Position);
    return distance < 0.1f;  // Considera raggiunta la destinazione se è sufficientemente vicino
}

void Enemy::CheckCollision(const std::vector<GameObject>& solidObjects)
{
    isOnGround = false;

    for (const GameObject& solid : solidObjects)
    {
        if (HasCollided(solid))
        {
            std::cout << "Collisione con terreno rilevata!" << std::endl;
            HandleCollision(solid);
        }
    }

    if (!isOnGround)
    {
        std::cout << "Nemico in aria" << std::endl;
        this->isMidAir = true;
    }
    else
    {
        std::cout << "Nemico a terra" << std::endl;
        this->isMidAir = false;
    }
}


bool Enemy::HasCollided(const GameObject& other) const {
    Hitbox bounds1 = this->GetHitbox();
    Hitbox bounds2 = other.GetHitbox();
    return (bounds1.Min.x <= bounds2.Max.x && bounds1.Max.x >= bounds2.Min.x &&
        bounds1.Min.y <= bounds2.Max.y && bounds1.Max.y >= bounds2.Min.y);
}

void Enemy::HandleCollision(const GameObject& solid)
{
    Hitbox enemyHitbox = this->GetHitbox();
    Hitbox solidHitbox = solid.GetHitbox();

    // Calcolo la compenetrazione per ogni asse
    float overlapX = std::min(enemyHitbox.Max.x, solidHitbox.Max.x) - std::max(enemyHitbox.Min.x, solidHitbox.Min.x);
    float overlapY = std::min(enemyHitbox.Max.y, solidHitbox.Max.y) - std::max(enemyHitbox.Min.y, solidHitbox.Min.y);

    // Correggo l'overlap minore
    if (overlapX < overlapY) // Correggo sull'asse X
    {
        if (this->Position.x < solid.Position.x) // Collisione a destra
        {
            this->Position.x -= overlapX;
        }
        else // Collisione a sinistra
        {
            this->Position.x += overlapX;
        }
        this->Velocity.x = Velocity.x;   //invertire velocity per ottenere effetto bouncing anche con le pareti
    }
    else // Correggi sull'asse Y
    {
        if (this->Position.y < solid.Position.y) // Collisione sopra
        {
            this->Position.y -= overlapY;
            this->isOnGround = true; // Il nemico è a terra
            this->isMidAir = false; // Resetta lo stato a terra
            this->isPastJumpPeak = false; // Resetta lo stato del salto
        }
        else // Collisione sotto
        {
            this->Position.y += overlapY;
        }
        this->Velocity.y = 0; // Ferma il movimento verticale
    }
}
