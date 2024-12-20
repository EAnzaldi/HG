#pragma once

#include "GameObject.h"

class MovingObject : public GameObject
{
public:
    bool isOnGround = true;

    float gravityForce = -3.0f;

    glm::vec2 velocity;

    MovingObject(glm::vec2 position, glm::vec2 size, TextureObject texture, bool repeatWidth, glm::vec2 velocity);

    void Move(float deltaTime);

    bool CheckCollision(const GameObject other);

    void CheckCollisionWithSolids(const std::vector<GameObject>& solidObjects);

    virtual void HandleCollisionWithSolid(GameObject solidObject);
};
