#pragma once

#include "MovingObject.h"

enum class EffectType { None, Speed, NoJump };

//associata alla texture della caramella run time
struct CandyType {
    EffectType effect;
    float value;
    float duration; // seconds

    CandyType();
    CandyType(EffectType effect, float duration);
    CandyType(EffectType effect, float value, float duration);
};

class Candy : public MovingObject {
public:
    bool IsEaten() { return Ate; }
    void Eat() { Ate = true; }
    CandyType GetType() { return type; }

    Candy(glm::vec2 position, glm::vec3 size, TextureObject* texture, bool repeatWidth, CandyType type);

    void Move(float deltaTime) override;
    void Render(const Shader& Shader) const override;


private:
    bool Ate = false;
    CandyType type;
};