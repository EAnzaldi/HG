#pragma once

#include "GameObject.h"

class MovingObject : public GameObject
{
public:

    enum class Collision { None, Top, Bottom, Left, Right };

    bool isOnGround = true;

    float gravityForce = -3.0f;

    glm::vec2 velocity;

    bool lastDirectionRight = true;  // Assume che inizialmente l'oggetto si muova a destra

    //Rotazione
    float rotationSpeed = 620.0f;      // Velocità della rotazione
    float targetRotation = 0.0f;    // Angolo di rotazione di destinazione

    //Oscillazione
    int totalSwings = 0;
    int nSwings = 0;

    //Costruttore 3d
    MovingObject(glm::vec2 position, glm::vec3 size, Model* model, bool repeatWidth, glm::vec2 velocity, bool moveRight);
    //Costruttore 2d
    MovingObject(glm::vec2 position, glm::vec3 size, TextureObject* texture, bool repeatWidth, glm::vec2 velocity, bool moveRight);

    virtual void Move(float deltaTime);

    void Rotate(float deltatime);

    bool Oscillate(float deltatime);

    void SetRotation(float rotation, glm::vec3 rotationAxis, glm::vec2 pivot, float timeSec);
    void SetRotation(float rotation, glm::vec3 rotationAxis, float timeSec);

    virtual void Render(const Shader& Shader) const;

    void Stop(float deltaTime);

    //bool CheckCollision(GameObject other);

    Collision CheckCollision(GameObject* other);

    void CheckCollisionWithSolids(const std::vector<GameObject*>& solidObjects);

protected:

    virtual void HandleCollisionWithSolid(GameObject* solidObject, Collision collision);

};