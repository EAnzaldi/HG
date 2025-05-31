#pragma once

#include "MovingObject.h"

class Enemy : public MovingObject {
public:

    bool IsDead() { return Dead; }
    void kill() { Dead = true; }
  
    Enemy(glm::vec2 position, glm::vec3 size, Model model, TextureObject* texture, bool repeatWidth, glm::vec2 velocity);

    void HandleCollisionWithSolid(GameObject solidObject) override;

bool Dead = false;

    private:
        
};
