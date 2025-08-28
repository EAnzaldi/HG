#pragma once

#include "MovingObject.h"
#include "GameObject.h"

class Enemy : public MovingObject {
public:
    bool IsDead() { return Dead; }
    void kill() { Dead = true; }
  
    //Costruttore 3d
    Enemy(glm::vec2 position, glm::vec3 size, Model* model, bool repeatWidth, glm::vec2 velocity, bool moveRight);
    //Costruttore 2d
    Enemy(glm::vec2 position, glm::vec3 size, TextureObject* texture, bool repeatWidth, glm::vec2 velocity, bool moveRight);

    void HandleCollisionWithSolid(GameObject* solidObject) override;

    bool Dead = false;

    void Move(float deltaTime) override;

    static void SpeedUp(float amount);
    static void SpeedDown(float amount);

    private:
        
};