#pragma once

#include "GameObject.h"

class Player : public GameObject {
public:
    glm::vec2 Velocity;

    Player(glm::vec2 position, glm::vec2 size, TextureObj texture)
        : GameObject(position, size, texture), Velocity(0.0f, 0.0f) {
    }

    void Move(float deltaTime) {
        this->Position += this->Velocity * deltaTime;
    }

    //mi sa che va spostato in game object per farlo funzionare anche con i nemici
    void CheckCollisions(const std::vector<GameObject>& solidObjects)
    {
        for (const GameObject& solid : solidObjects)
        {
            if (CheckCollision(solid))
            {
                HandleCollisionWithSolidObject(solid);
            }
        }
    }

    void HandleCollisionWithSolidObject(const GameObject& solid)
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
};
