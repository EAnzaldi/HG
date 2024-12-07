#pragma once

#include "GameObject.h"
#include "Enemy.h"

class Player : public GameObject {
public:
    glm::vec2 Velocity;

    Player(glm::vec2 position, glm::vec2 size, TextureObj texture, bool repeatWidth);


    void Move(float deltaTime);
    //Collisions with enemy
    bool CheckCollision(const Enemy& enemy);
    //Collisions with platforms
    void CheckCollision(const std::vector<GameObject>& solidObjects);
    bool IsCollided(const GameObject& other) const;
    void HandleCollision(const GameObject& solid);
    

private:
};
