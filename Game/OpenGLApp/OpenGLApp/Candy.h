#ifndef CANDY_H
#define CANDY_H

#include "MovingObject.h"

enum class CandyType { Bonus1, Bonus2, Malus1 };

class Candy : public MovingObject {
public:
    CandyType type;
    int duration; // seconds

    Candy(glm::vec2 position, glm::vec3 size, FlatMesh* fmesh, bool repeatWidth);

private:

};

#endif