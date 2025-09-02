#pragma once

#include "MovingObject.h"
#include "Enemy.h"
#include "Candy.h"

#include <irrKlang.h>

struct ActiveEffect {
    CandyType type;
    float remainingTime;
    bool active;

    ActiveEffect(CandyType type)
        : type(type), remainingTime(type.duration), active(true) {
    }
};

enum class PlayerName { Gretel, Hansel };

class Player : public MovingObject
{
    public:
        PlayerName name;

        bool isInvincible;           // Flag per indicare se il giocatore è invincibile
        float invincibilityDuration; // Durata dell'invincibilità (in secondi)
        float invincibilityTimer;    // Timer corrente per l'invincibilità

        bool isDead = false;    //Player inizia vivo
        int lives;          //Player inizia con 3 vite

        float baseJumpForce = 1.5f;         // Forza iniziale del salto

        bool isPastJumpPeak = false;
        float addedJumpForce = 2.5f;        // Forza aggiuntiva durante il salto (tenendo premuto il tasto)
        float maxJumpTime = 0.3f;           // Durata massima del salto (tenendo premuto il tasto)
        float currentJumpTime = 0.0f;       // Tempo trascorso dall'inizio del salto

        glm::vec2 maxVelocity = glm::vec2(1.5f, 2.5f);

        std::vector<ActiveEffect*> pAEffects;

        //Costruttore 3d
        Player(glm::vec2 position, glm::vec3 size, Model* model, bool repeatWidth, PlayerName name, int lives);
        //Costruttore 2d
        Player(glm::vec2 position, glm::vec3 size, TextureObject* texture, bool repeatWidth, PlayerName name, int lives);

        void HandleJump(float deltaTime, irrklang::ISoundEngine* engine);

        void HandleCollisionWithSolid(GameObject* solidObject) override;

        bool CheckEnemyCollision(Enemy* enemy, irrklang::ISoundEngine* engine);   

        bool CheckCandyCollision(Candy* candy, irrklang::ISoundEngine* engine);

        void Update(float deltaTime); // Aggiorna lo stato del giocatore
        
        // Avviano l'invincibilità
        void StartTempInvincibility(); 
        void StartInvincibility();
        void EndInvincibility();

        void EatCandy(CandyType type, irrklang::ISoundEngine* engine);

        void GetStats(std::vector<CandyType*> pCandyTypes, std::vector<int>& candyStats, int& kills);

        void Teleport(glm::vec2 position, irrklang::ISoundEngine* engine);
        static bool teleport;

    private:
        bool disableJump = false;
        int nNoJump = 0;
        int nInvincibility = 0;

        int nNoJumpEaten = 0;
        int nSpeedEaten = 0;
        int nSpeedEnemyEaten = 0;
        int nInvincibilityEaten = 0;
        int nTeleportEaten = 0;

        int nKills = 0;

        void DigestCandy(CandyType type);

};
