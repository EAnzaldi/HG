#include "Player.h"

Player::Player(glm::vec2 position, glm::vec3 size, Model* model, bool repeatWidth, PlayerName name)
    : MovingObject(position, size, model, repeatWidth, velocity = glm::vec2(0.0f, 0.0f), true), 
    invincibilityDuration(1.0f), invincibilityTimer(0.0f), isInvincible(false), name(name)
{

}
Player::Player(glm::vec2 position, glm::vec3 size, TextureObject* texture, bool repeatWidth, PlayerName name)
    : MovingObject(position, size, texture, repeatWidth, velocity = glm::vec2(0.0f, 0.0f), true),
        invincibilityDuration(1.0f), invincibilityTimer(0.0f), isInvincible(false), name(name)
{

}
void Player::HandleJump(float deltaTime, irrklang::ISoundEngine* engine)
{
    if (disableJump)
        if(isOnGround)
            return;
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
bool Player::CheckEnemyCollision(Enemy* enemy, irrklang::ISoundEngine* engine)
{
    if (!isInvincible)
    {
        Collision collision = CheckCollision(enemy);

        if (collision == Collision::Other) {
            engine->play2D("resources/sounds/damage.wav");
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
        else if (collision == Collision::Top) {

            //std::cout << "Kill enemy" << std::endl;
            engine->play2D("resources/sounds/kill_slime.wav");
            enemy->kill();
        }
    }
    return isDead;
}
bool Player::CheckCandyCollision(Candy* candy, irrklang::ISoundEngine* engine)
{
    Collision collision = CheckCollision(candy);

    if (collision != Collision::None) {
        return true;
    }
    return false;
}
void Player::EatCandy(CandyType type, irrklang::ISoundEngine* engine)
{
    engine->play2D("resources/sounds/crunch2.ogg");
    switch (type.effect) {
        case(EffectType::None): {} break;
        case(EffectType::NoJump): disableJump = true; nNoJump++; break;
        case(EffectType::Speed): maxVelocity *= type.value; break;
        case(EffectType::SpeedEnemy): Enemy::SpeedUp(type.value); break;
        case(EffectType::Invincibility): isInvincible = true; nInvincibility++; printf("INVINCIBLE!\n"); break;
        case(EffectType::Teleport): teleport = true; return;//non viene inserito nel vettore degli effetti
        default: return;
    }
    ActiveEffect* pe = new ActiveEffect(type);
    pAEffects.emplace_back(pe);
}
void Player::DigestCandy(CandyType type)
{
    switch (type.effect) {
        case(EffectType::None): {} break;
        case(EffectType::NoJump): nNoJump--; break;
        case(EffectType::Speed): maxVelocity /= type.value; break;
        case(EffectType::SpeedEnemy): Enemy::SpeedDown(type.value); break;
        case(EffectType::Invincibility): nInvincibility--; break;
        case(EffectType::Teleport): {} break;
        default: return;
    }
    //I flag non cumulabili vengono ripristinati solo quando tutti gli effetti che li alterano sono scaduti
    if(nNoJump <= 0)
        disableJump = false;
    if (nInvincibility <= 0) {
        isInvincible = false;
        printf("Invincibility ended\n");
    }
}
void Player::Teleport(glm::vec2 position, irrklang::ISoundEngine* engine) {
    Position = position;
    teleport = false;
    engine->play2D("resources/sounds/teleport.wav");
}
void Player::HandleCollisionWithSolid(GameObject* solidObject)
{
    Hitbox playerHitbox = this->GetHitbox();
    Hitbox solidHitbox = solidObject->GetHitbox();

    float overlapX = std::min(playerHitbox.Max.x, solidHitbox.Max.x) - std::max(playerHitbox.Min.x, solidHitbox.Min.x);
    float overlapY = std::min(playerHitbox.Max.y, solidHitbox.Max.y) - std::max(playerHitbox.Min.y, solidHitbox.Min.y);

    // Correggo l'overlap minore
    if (overlapX < overlapY) // Correggo sull'asse X
    {
        if (this->Position.x < solidObject->Position.x) // Collisione a destra del player
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
        if (this->Position.y < solidObject->Position.y) // Collisione sopra il player
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
    if (isInvincible && nInvincibility==0)
    {
        invincibilityTimer += deltaTime;
        if (invincibilityTimer >= invincibilityDuration)
        {
            isInvincible = false;  // Fine invincibilità
            invincibilityTimer = 0.0f;
            std::cout << "Invincibility ended" << std::endl;
        }
    }

    bool erase = false;
    if (pAEffects.size() != 0) {
        // Altri aggiornamenti del giocatore
        for (ActiveEffect* pe : pAEffects) {
            pe->remainingTime -= deltaTime;
            if (pe->remainingTime <= 0) {
                DigestCandy(pe->type);
                erase = true;
            }  
        }
        //Rimuove gli elementi nel vettore che soddisfano la condizione tempo restante <= 0
        if(erase)
            pAEffects.erase(std::remove_if(pAEffects.begin(), pAEffects.end(),
                                        [](const ActiveEffect* pe) { return pe->remainingTime <= 0.0f; }),
                        pAEffects.end());

    }   
}

void Player::StartInvincibility()
{
    isInvincible = true;
    invincibilityTimer = 0.0f; // Resetta il timer
}

