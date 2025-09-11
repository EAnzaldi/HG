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

enum class Collision { None, Top, Bottom, Left, Right };

class Player : public MovingObject
{
    public:
        PlayerName name;
        const char* nameString;

        bool isInvincible;           // Flag per indicare se il giocatore è invincibile
        float invincibilityDuration; // Durata dell'invincibilità (in secondi)
        float invincibilityTimer;    // Timer corrente per l'invincibilità

        bool isDead = false;    //Player inizia vivo
        int lives;

        float baseJumpForce = 1.5f;         // Forza iniziale del salto

        bool isPastJumpPeak = false;
        float addedJumpForce = 2.5f;        // Forza aggiuntiva durante il salto (tenendo premuto il tasto)
        float maxJumpTime = 0.3f;           // Durata massima del salto (tenendo premuto il tasto)
        float currentJumpTime = 0.0f;       // Tempo trascorso dall'inizio del salto

        glm::vec2 maxVelocity = glm::vec2(1.5f, 2.5f);

        std::vector<ActiveEffect*> pAEffects = {};

        //Costruttore 3d
        Player(glm::vec2 position, glm::vec3 size, Model* model, bool repeatWidth, PlayerName name, int lives);
        //Costruttore 2d
        Player(glm::vec2 position, glm::vec3 size, TextureObject* texture, bool repeatWidth, PlayerName name, int lives);

        ~Player();

        void HandleJump(float deltaTime, irrklang::ISoundEngine* engine);


        bool CheckEnemyCollision(Enemy* enemy, irrklang::ISoundEngine* engine);   

        //bool CheckCandyCollision(Candy* candy, irrklang::ISoundEngine* engine);

        Candy* CheckCollisionWithCandies(const std::vector<Candy*>& candies);
        std::string EatCandy(CandyType type, irrklang::ISoundEngine* engine);

        void Update(float deltaTime); // Aggiorna lo stato del giocatore
        
        // Avviano l'invincibilità
        void StartTempInvincibility(); 
        void StartInvincibility();
        void EndInvincibility();

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

        void HandleCollisionWithSolid(GameObject* solidObject, Collision collision) override;

};
