#pragma once

#include "GameObject.h"

class Enemy : public GameObject {
public:
    glm::vec2 Velocity;
    glm::vec2 Destination;  // Destinazione del nemico
    
    bool isOnGround = false;
    bool isMidAir = false;
    bool isPastJumpPeak = false;

    float gravityForce = -3.0f;         // Forza di gravità
    float baseJumpForce = 1.5f;         // Forza iniziale del salto
    float addedJumpForce = 2.5f;        // Forza aggiuntiva durante il salto (tenendo premuto il tasto)
    float maxJumpTime = 0.3f;           // Durata massima del salto (tenendo premuto il tasto)
    float currentJumpTime = 0.0f;       // Tempo trascorso dall'inizio del salto

    glm::vec2 MaxVelocity = glm::vec2(1.5f, 2.5f);

    Enemy(glm::vec2 position, glm::vec2 size, TextureObject texture, bool repeatWidth);

    void SetDestination(glm::vec2 destination);  // Imposta la destinazione
    
    void CheckCollision(const std::vector<GameObject>& solidObjects);

    bool HasCollided(const GameObject& other) const;

    void HandleCollision(const GameObject& solid);

    void Move(float deltaTime);

    void HandleJump(float deltaTime);
    
    bool HasReachedDestination() const;  // Verifica se ha raggiunto la destinazione
};
