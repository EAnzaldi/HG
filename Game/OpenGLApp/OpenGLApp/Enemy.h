#pragma once

#include "MovingObject.h"

class Enemy : public MovingObject {
public:
  
    Enemy(glm::vec2 position, glm::vec3 size, Model model, TextureObject* texture, bool repeatWidth, glm::vec2 velocity);
  
    void Move(float deltaTime);

};
