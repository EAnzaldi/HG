#ifndef CANDY_H
#define CANDY_H

#include "MovingObject.h"

enum class CandyType { Bonus1, Bonus2, Malus1 };

class Candy : public MovingObject {
public:
    CandyType type;
    int duration; // seconds

    bool Ate = false;

    bool IsAte() { return Ate; }
    void Eat() { Ate = true; }

    Candy(glm::vec2 position, glm::vec3 size, FlatMesh* fmesh, bool repeatWidth);

    void Move(float deltaTime) override;
    void Render(const Shader& Shader) const override;
};

#endif