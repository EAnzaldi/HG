#pragma once

#include "GameObject.h"

class MovingObject : public GameObject
{
public:
    bool isOnGround = true;

    float gravityForce = -3.0f;

    glm::vec2 velocity;

    bool lastDirectionRight = true;  // Assume che inizialmente l'oggetto si muova a destra
    float rotationSpeed = 720.0f;      // Velocit� della rotazione
    float targetRotation = 0.0f;    // Angolo di rotazione di destinazione

    MovingObject(glm::vec2 position, glm::vec3 size, Model model, TextureObject texture, bool repeatWidth, glm::vec2 velocity);

    void Move(float deltaTime);

    bool CheckCollision(const GameObject other);

    void CheckCollisionWithSolids(const std::vector<GameObject>& solidObjects);

    virtual void HandleCollisionWithSolid(GameObject solidObject);
};
