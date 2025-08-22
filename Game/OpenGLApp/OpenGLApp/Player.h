#pragma once

#include "MovingObject.h"
#include "Enemy.h"
#include "Candy.h"

#include <irrKlang.h>

class Player : public MovingObject
{
    public:
        bool isInvincible;           // Flag per indicare se il giocatore è invincibile
        float invincibilityDuration; // Durata dell'invincibilità (in secondi)
        float invincibilityTimer;    // Timer corrente per l'invincibilità

        bool isDead = false;    //Player inizia vivo
        int lives = 3;          //Player inizia con 3 vite

        float baseJumpForce = 1.5f;         // Forza iniziale del salto

        bool isPastJumpPeak = false;
        float addedJumpForce = 2.5f;        // Forza aggiuntiva durante il salto (tenendo premuto il tasto)
        float maxJumpTime = 0.3f;           // Durata massima del salto (tenendo premuto il tasto)
        float currentJumpTime = 0.0f;       // Tempo trascorso dall'inizio del salto

        glm::vec2 maxVelocity = glm::vec2(1.5f, 2.5f);

        //Costruttore 3d
        Player(glm::vec2 position, glm::vec3 size, Model* model, TextureObject* texture, bool repeatWidth);
        //Costruttore 2d
        Player(glm::vec2 position, glm::vec3 size, FlatMesh* fmesh, bool repeatWidth);

        void HandleJump(float deltaTime, irrklang::ISoundEngine* engine);

        void HandleCollisionWithSolid(GameObject* solidObject) override;

        bool CheckEnemyCollision(Enemy* enemy, irrklang::ISoundEngine* engine);   

        void CheckCandyCollision(Candy* candy, irrklang::ISoundEngine* engine);

        void Update(float deltaTime); // Aggiorna lo stato del giocatore
        
        void StartInvincibility();    // Avvia l'invincibilità

    private:
};
