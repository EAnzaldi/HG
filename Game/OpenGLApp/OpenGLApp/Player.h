#pragma once

#include "GameObject.h"
#include "Enemy.h"

#include <irrKlang.h>

class Player : public GameObject 
{
    public:
        float invincibilityDuration; // Durata dell'invincibilitā (in secondi)
        float invincibilityTimer;    // Timer corrente per l'invincibilitā
        bool isInvincible;           // Flag per indicare se il giocatore č invincibile

        bool isDead = false;    //Player inizia vivo
        int lives = 3;          //Player inizia con 3 vite

        bool isOnGround = false;
        bool isMidAir = false;
        bool isPastJumpPeak = false;

        float gravityForce = -3.0f;         // Forza di gravitā
        float baseJumpForce = 1.5f;         // Forza iniziale del salto
        float addedJumpForce = 2.5f;        // Forza aggiuntiva durante il salto (tenendo premuto il tasto)
        float maxJumpTime = 0.3f;           // Durata massima del salto (tenendo premuto il tasto)
        float currentJumpTime = 0.0f;       // Tempo trascorso dall'inizio del salto

        glm::vec2 Velocity;

        glm::vec2 MaxVelocity = glm::vec2(1.5f, 2.5f);

        Player(glm::vec2 position, glm::vec2 size, TextureObject texture, bool repeatWidth);

        void Move(float deltaTime);

        void HandleJump(float deltaTime, irrklang::ISoundEngine* engine);

        bool CheckCollision(const Enemy& enemy);

        void CheckCollision(const std::vector<GameObject>& solidObjects);

        bool HasCollided(const GameObject& other) const;

        void HandleCollision(const GameObject& solid);

        void Update(float deltaTime); // Aggiorna lo stato del giocatore
        
        void StartInvincibility();    // Avvia l'invincibilitā

    private:
};
