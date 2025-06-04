#include "MovingObject.h"

MovingObject::MovingObject(glm::vec2 position, glm::vec3 size, Model model, TextureObject* texture, bool repeatWidth, glm::vec2 velocity, bool moveRight)
    : GameObject(position, size, model, texture, repeatWidth), velocity(velocity), lastDirectionRight(moveRight) {
}

void MovingObject::Move(float deltaTime)
{
    if (!isOnGround)
    {
        this->velocity.y += gravityForce * deltaTime;
    }

    this->Position += this->velocity * deltaTime;

    if (this->Position.x - this->Size.x / 2 >= 1.0f) {
        this->Position.x = -1.0f + this->Size.x / 2;
    }

    if (this->Position.x + this->Size.x / 2 <= -1.0f) {
        this->Position.x = 1.0f - this->Size.x / 2;
    }


    if (velocity.x > 0.0f && !lastDirectionRight) {
        targetRotation = 0.0f;  // Direzione verso destra
        lastDirectionRight = true;
    }
    else if (velocity.x < 0.0f && lastDirectionRight) {
        targetRotation = 180.0f;  // Direzione verso sinistra
        lastDirectionRight = false;
    }

    // Se serve applico la rotazione del modello
    /*
    if (std::abs(targetRotation - Rotation) > 0.1f) {
        float rotationStep = rotationSpeed * deltaTime;

        rotationStep = std::min(rotationStep, 180.0f);  // Limito a 180 gradi per evitare overshoot

        if (std::abs(targetRotation - Rotation) < rotationStep) {
            Rotation = targetRotation;  // Allineo la rotazione alla destinazione se la differenza è piccola
        }
        else {
            Rotation += (targetRotation > Rotation ? rotationStep : -rotationStep);
        }
    }*/

    FlipX = lastDirectionRight ? 1.0f : -1.0f;
}

/*bool MovingObject::CheckCollision(GameObject other)
{
    Hitbox bounds1 = this->GetHitbox();
    Hitbox bounds2 = other.GetHitbox();
    return (bounds1.Min.x <= bounds2.Max.x && bounds1.Max.x >= bounds2.Min.x &&
        bounds1.Min.y <= bounds2.Max.y && bounds1.Max.y >= bounds2.Min.y);
}*/

MovingObject::Collision MovingObject::CheckCollision(GameObject other)
{
    Hitbox bounds1 = this->GetHitbox();
    Hitbox bounds2 = other.GetHitbox();

    bool isColliding = (bounds1.Min.x <= bounds2.Max.x && bounds1.Max.x >= bounds2.Min.x &&
        bounds1.Min.y <= bounds2.Max.y && bounds1.Max.y >= bounds2.Min.y);

    if(!isColliding)
        return Collision::None;

    if (this->velocity.y < 0) {// se player sta cadendo su other
        if (bounds1.Min.y <= bounds2.Max.y && bounds1.Min.y >= bounds2.Max.y - 5.0f)//se colpisce entro una finestra di 5.0f
            return Collision::Top;
    }

    return Collision::Other;
}

void MovingObject::CheckCollisionWithSolids(const std::vector<GameObject>& solidObjects)
{
    this->isOnGround = false;   // Considero l'oggetto a mezz'aria prima del controllo delle collisioni

    for (const GameObject& solid : solidObjects)
    {  
        if (CheckCollision(solid) != Collision::None)
        {
            HandleCollisionWithSolid(solid);
        }
    }
}

void MovingObject::HandleCollisionWithSolid(GameObject solidObject)
{
    Hitbox thisHitbox = this->GetHitbox();
    Hitbox solidHitbox = solidObject.GetHitbox();

    float overlapX = std::min(thisHitbox.Max.x, solidHitbox.Max.x) - std::max(thisHitbox.Min.x, solidHitbox.Min.x);
    float overlapY = std::min(thisHitbox.Max.y, solidHitbox.Max.y) - std::max(thisHitbox.Min.y, solidHitbox.Min.y);

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

void MovingObject::Render(const Shader& Shader) const
{
    GameObject::Render(Shader);

    // Se l'oggetto si trova oltre il bordo a destra renderizzo la parte mancante a sinistra
    if (this->Position.x >= 1.0f - this->Size.x / 2)
    {
        glm::mat4 model_mat = glm::mat4(1.0f);
        model_mat = glm::translate(model_mat, glm::vec3((this->Position - glm::vec2(2.0f, 0.0f)), 0.0f));
        model_mat = glm::rotate(model_mat, glm::radians(this->Rotation), glm::vec3(0.0f, 1.0f, 0.0f)); // Ruota attorno all'asse Z
        //model_mat = glm::scale(model_mat, this->Size);
        model_mat = glm::scale(model_mat, glm::vec3(FlipX * this->Size.x, this->Size.y, 1.0f));

        Shader.setMat4("model", model_mat);

        model.Draw(Shader);
    }

    // Se l'oggetto si trova oltre il bordo a sinistra renderizzo la parte mancante a destra
    if (this->Position.x <= -1.0f + this->Size.x / 2)
    {
        glm::mat4 model_mat = glm::mat4(1.0f);
        model_mat = glm::translate(model_mat, glm::vec3((this->Position + glm::vec2(2.0f, 0.0f)), 0.0f));
        model_mat = glm::rotate(model_mat, glm::radians(this->Rotation), glm::vec3(0.0f, 1.0f, 0.0f)); // Ruota attorno all'asse Z
        //model_mat = glm::scale(model_mat, this->Size);
        model_mat = glm::scale(model_mat, glm::vec3(FlipX * this->Size.x, this->Size.y, 1.0f));

        Shader.setMat4("model", model_mat);

        model.Draw(Shader);
    }
}