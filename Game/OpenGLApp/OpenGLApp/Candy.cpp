#include "Candy.h"

Candy::Candy(glm::vec2 position, glm::vec3 size, Model* model, TextureObject* texture, bool repeatWidth, glm::vec2 velocity, bool moveRight)
    : MovingObject(position, size, model, texture, repeatWidth, velocity, moveRight) {
}