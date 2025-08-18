#include "Candy.h"

Candy::Candy(glm::vec2 position, glm::vec3 size, FlatMesh* fmesh, bool repeatWidth)
    : MovingObject(position, size, fmesh, repeatWidth, velocity = glm::vec2(0.0f, 0.0f), false) {
}