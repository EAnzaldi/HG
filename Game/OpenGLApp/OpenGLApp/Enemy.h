#pragma once

#include "MovingObject.h"
#include "GameObject.h"

enum class EnemyType { Slime, SuperSlime };

class Enemy : public MovingObject {
public:

    EnemyType type;

    bool IsDead() { return isDead; }
    void hit() {
        if(lives>0)
            lives--;
        if (lives <= 0)
            isDead = true;            
    }
    int getLives() { return lives; };
 
    //Costruttore 3d
    Enemy(glm::vec2 position, glm::vec3 size, Model* model, bool repeatWidth, glm::vec2 velocity, bool moveRight, EnemyType type);
    //Costruttore 2d
    Enemy(glm::vec2 position, glm::vec3 size, TextureObject* texture, bool repeatWidth, glm::vec2 velocity, bool moveRight, EnemyType type);

    //void HandleCollisionWithSolid(GameObject* solidObject) override;

    void Move(float deltaTime) override;

    static void SpeedUp(float amount);
    static void SpeedDown(float amount);

private:

    bool isDead = false;    //Enemy inizia vivo
    int lives;

    void Flip();
    static float speedBonus;  

    void HandleCollisionWithSolid(GameObject* solidObject, Collision collision) override;
};