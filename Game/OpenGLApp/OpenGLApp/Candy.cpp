#include "Candy.h"

CandyType::CandyType()
    : effect(EffectType::None), value(0.0f), duration(0.0f) {
}
CandyType::CandyType(EffectType effect)
    : effect(effect), value(0.0f), duration(0.0f) {
}
CandyType::CandyType(EffectType effect, float duration)
    : effect(effect), value(0.0f), duration(duration) {
}
CandyType::CandyType(EffectType effect, float value, float duration)
    : effect(effect), value(value), duration(duration) {
}
Candy::Candy(glm::vec2 position, glm::vec3 size, TextureObject* texture, bool repeatWidth, CandyType type)
    : MovingObject(position, size, texture, repeatWidth, velocity = glm::vec2(0.0f, 0.0f), false), type(type) {
}
void Candy::Move(float deltaTime)
{
    if (!isOnGround)
    {
        this->velocity.y += gravityForce * deltaTime;
    }

    this->Position += this->velocity * deltaTime;
}
void Candy::Render(const Shader& Shader) const
{
    GameObject::Render(Shader);
}