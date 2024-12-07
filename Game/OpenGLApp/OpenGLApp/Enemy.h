#pragma once

#include "GameObject.h"

class Enemy : public GameObject {
public:
    glm::vec2 Velocity;

    Enemy(glm::vec2 position, glm::vec2 size, TextureObj texture, bool repeatWidth);


    void Move(float deltaTime);

private:
};

