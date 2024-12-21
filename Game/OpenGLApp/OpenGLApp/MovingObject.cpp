#include "MovingObject.h"

MovingObject::MovingObject(glm::vec2 position, glm::vec3 size, Model model, TextureObject* texture, bool repeatWidth, glm::vec2 velocity)
    : GameObject(position, size, model, texture, repeatWidth), velocity(velocity) {
}

void MovingObject::Move(float deltaTime)
{
    if (!isOnGround)
    {
        this->velocity.y += gravityForce * deltaTime;
    }

    this->Position += this->velocity * deltaTime;

    if (this->Position.x > 1.0f - this->Size.x) {
        this->Position.x = 1.0f - this->Size.x;
    }

    if (this->Position.x < -1.0f + this->Size.x) {
        this->Position.x = -1.0f + this->Size.x;
    }

    if (velocity.x > 0.0f && !lastDirectionRight) {
        targetRotation = 0.0f;  // Direzione verso destra
        lastDirectionRight = true;
    }
    else if (velocity.x < 0.0f && lastDirectionRight) {
        targetRotation = 180.0f;  // Direzione verso sinistra
        lastDirectionRight = false;
    }

    // Interpola la rotazione più velocemente
    if (std::abs(targetRotation - Rotation) > 0.1f) {
        float rotationStep = rotationSpeed * deltaTime;

        // Maggiore velocità di rotazione
        rotationStep = std::min(rotationStep, 180.0f);  // Limita a 180 gradi per evitare overshoot

        if (std::abs(targetRotation - Rotation) < rotationStep) {
            Rotation = targetRotation;  // Allinea la rotazione alla destinazione se la differenza è piccola
        }
        else {
            Rotation += (targetRotation > Rotation ? rotationStep : -rotationStep);  // Ruota gradualmente
        }
    }
}

bool MovingObject::CheckCollision(GameObject other)
{
    Hitbox bounds1 = this->GetHitbox();
    Hitbox bounds2 = other.GetHitbox();
    return (bounds1.Min.x <= bounds2.Max.x && bounds1.Max.x >= bounds2.Min.x &&
        bounds1.Min.y <= bounds2.Max.y && bounds1.Max.y >= bounds2.Min.y);
}

void MovingObject::CheckCollisionWithSolids(const std::vector<GameObject>& solidObjects)
{
    this->isOnGround = false;   // Considero l'oggetto a mezz'aria prima del controllo delle collisioni

    for (const GameObject& solid : solidObjects)
    {  
        if (CheckCollision(solid))
        {
            HandleCollisionWithSolid(solid);
        }
    }
}

void MovingObject::HandleCollisionWithSolid(GameObject solidObject)
{
    Hitbox playerHitbox = this->GetHitbox();
    Hitbox solidHitbox = solidObject.GetHitbox();

    float overlapX = std::min(playerHitbox.Max.x, solidHitbox.Max.x) - std::max(playerHitbox.Min.x, solidHitbox.Min.x);
    float overlapY = std::min(playerHitbox.Max.y, solidHitbox.Max.y) - std::max(playerHitbox.Min.y, solidHitbox.Min.y);

    // Correggo l'overlap minore
    if (overlapX < overlapY) // Correggo sull'asse X
    {
        if (this->Position.x < solidObject.Position.x) // Collisione a destra del MovingObject
        {
            this->Position.x -= overlapX;
        }
        else // Collisione a sinistra del MovingObject
        {
            this->Position.x += overlapX;
        }
    }
    else // Correggi sull'asse Y
    {
        if (this->Position.y < solidObject.Position.y) // Collisione sopra del MovingObject
        {
            this->Position.y -= overlapY;
        }
        else // Collisione sotto del MovingObject
        {
            this->Position.y += overlapY;
            this->isOnGround = true;
        }
        this->velocity.y = 0;
    }
}