#pragma once

#include "MovingObject.h"

class Enemy : public MovingObject {
public:
  
    Enemy(glm::vec2 position, glm::vec2 size, TextureObject texture, bool repeatWidth, glm::vec2 velocity);
  
    void Move(float deltaTime);

};
