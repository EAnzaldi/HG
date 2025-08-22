#pragma once

#include "GameObject.h"

class MovingObject : public GameObject
{
public:

    enum class Collision { None, Top, Other };

    bool isOnGround = true;

    float gravityForce = -3.0f;

    glm::vec2 velocity;

    bool lastDirectionRight = true;  // Assume che inizialmente l'oggetto si muova a destra
    float rotationSpeed = 720.0f;      // Velocità della rotazione
    float targetRotation = 0.0f;    // Angolo di rotazione di destinazione

    //Costruttore 3d
    MovingObject(glm::vec2 position, glm::vec3 size, Model* model, TextureObject* texture, bool repeatWidth, glm::vec2 velocity, bool moveRight);
    //Costruttore 2d
    MovingObject(glm::vec2 position, glm::vec3 size, FlatMesh* fmesh, bool repeatWidth, glm::vec2 velocity, bool moveRight);

    virtual void Move(float deltaTime);

    virtual void Render(const Shader& Shader) const;

    //bool CheckCollision(GameObject other);

    Collision CheckCollision(GameObject* other);

    void CheckCollisionWithSolids(const std::vector<GameObject*>& solidObjects);

    virtual void HandleCollisionWithSolid(GameObject* solidObject);

};