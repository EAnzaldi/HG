#include "Enemy.h"
#include <glm/glm.hpp>  // Per il calcolo dei vettori

Enemy::Enemy(glm::vec2 position, glm::vec3 size, Model model, TextureObject* texture, bool repeatWidth, glm::vec2 velocity)
    : MovingObject(position, size, model, texture, repeatWidth, velocity) {
}

void Enemy::Move(float deltaTime)
{
    if (!isOnGround)
    {
        this->velocity.y += gravityForce * deltaTime;
    }

    this->Position.x += (this->velocity.x * deltaTime)/4;   //in player viene usato solo this->Position mentre qui voglio che la sua x
    this->Position.y += this->velocity.y * deltaTime;       //sia più lenta della y che deve rimanere fissa essendo la forza di gravità

    // Limita il movimento orizzontale ai bordi dello schermo (opzionale)
    if (this->Position.x > 1.0f - this->Size.x / 2) {
        this->Position.x = 1.0f - this->Size.x / 2;
        this->velocity.x = -std::abs(this->velocity.x); // Inverti direzione orizzontale
    }
    if (this->Position.x < -1.0f + this->Size.x / 2) {
        this->Position.x = -1.0f + this->Size.x / 2;
        this->velocity.x = std::abs(this->velocity.x); // Inverti direzione orizzontale
    }

    if (velocity.x > 0.0f && !lastDirectionRight) {
        targetRotation = 0.0f;  // Direzione verso destra
        lastDirectionRight = true;
    }
    else if (velocity.x < 0.0f && lastDirectionRight) {
        targetRotation = 180.0f;  // Direzione verso sinistra
        lastDirectionRight = false;
    }

    // Se serve applico la rotazione del modello
    if (std::abs(targetRotation - Rotation) > 0.1f) {
        float rotationStep = rotationSpeed * deltaTime;

        rotationStep = std::min(rotationStep, 180.0f);  // Limito a 180 gradi per evitare overshoot

        if (std::abs(targetRotation - Rotation) < rotationStep) {
            Rotation = targetRotation;  // Allineo la rotazione alla destinazione se la differenza è piccola
        }
        else {
            Rotation += (targetRotation > Rotation ? rotationStep : -rotationStep);
        }
    }
}