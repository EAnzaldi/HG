#include "MovingObject.h"

MovingObject::MovingObject(glm::vec2 position, glm::vec3 size, Model* model, bool repeatWidth, glm::vec2 velocity, bool moveRight)
    : GameObject(position, size, model, repeatWidth), velocity(velocity), lastDirectionRight(moveRight)
{
    // flag specifica se si voglia scalare la texture (consigliato=1 per piattaforme)
}
MovingObject::MovingObject(glm::vec2 position, glm::vec3 size, TextureObject* texture, bool repeatWidth, glm::vec2 velocity, bool moveRight)
    : GameObject(position, size, texture, repeatWidth), velocity(velocity), lastDirectionRight(moveRight)
{
    
}
void MovingObject::Move(float deltaTime)
{
    if (!isOnGround)
    {
        this->velocity.y += gravityForce * deltaTime;
    }

    this->Position += this->velocity * deltaTime;

    //Teletrasporto come pacman
    if (this->Position.x - this->Size.x / 2 >= 1.0f) {
        this->Position.x = -1.0f + this->Size.x / 2;
    }

    if (this->Position.x + this->Size.x / 2 <= -1.0f) {
        this->Position.x = 1.0f - this->Size.x / 2;
    }

}
void MovingObject::SetRotation(float rotation, glm::vec3 rotationAxis, glm::vec2 pivot, float timeSec) {
    Pivot = pivot;
    usePivot = true;
    Axis = rotationAxis;
    targetRotation = targetSwings = rotation;
    rotationSpeed = rotation / timeSec;
    printf("Pivot = %f %f", pivot.x, pivot.y);
}
void MovingObject::SetRotation(float rotation, glm::vec3 rotationAxis, float timeSec) {
    usePivot = false;
    Axis = rotationAxis;
    targetRotation = targetSwings = rotation;
    rotationSpeed = rotation / timeSec;
}
void MovingObject::Rotate(float deltatime) {
    // Ribaltamento del modello
    /*
    if (velocity.x > 0.0f && !lastDirectionRight) {
        targetRotation = 0.0f;  // Direzione verso destra
        lastDirectionRight = true;
    }
    else if (velocity.x < 0.0f && lastDirectionRight) {
        targetRotation = 180.0f;  // Direzione verso sinistra
        lastDirectionRight = false;
    }

    if (velocity.x > 0.0f) {
        targetRotation = 0.0f;  // Direzione verso destra
    } 
    else if (velocity.x < 0.0f) {
        targetRotation = 180.0f;  // Direzione verso sinistra
    }*/

    float remaining = targetRotation - Rotation;

    if (std::abs(remaining) > 0.1f) {
        float rotationStep = rotationSpeed * deltatime;

        rotationStep = std::min(rotationStep, 180.0f);  // Limito a 180 gradi per evitare overshoot

        if (std::abs(remaining) < rotationStep) {
            Rotation = targetRotation;  // Allineo la rotazione alla destinazione se la differenza è piccola
        }
        else {
            /*
            if(targetRotation > Rotation)//gestisce movimento orario/antiorario
                Rotation += remaining > 0 ? rotationStep : -rotationStep;
            else
                Rotation -= remaining > 0 ? rotationStep : -rotationStep;*/
            Rotation += (remaining > 0 ? rotationStep : -rotationStep);

        }
        if (Rotation > 180.0f) Rotation -= 360.0f;
        if (Rotation < -180.0f) Rotation += 360.0f;
    }

}
bool MovingObject::Oscillate(float deltatime) {
    if (nSwings > totalSwings) {
        //reset
        nSwings = 0;
        targetRotation = targetSwings;
        return false;
    }
    if (std::abs(Rotation - targetRotation) < 0.1f) {
        targetRotation = -targetRotation;
        if(nSwings==totalSwings-1)
            targetRotation=0.0f;    
        nSwings++;
    }
    Rotate(deltatime);
    return true;
}
void MovingObject::Stop(float deltaTime) {
    this->velocity.x = 0.0f;
}
//Collision::x --> this è stato colpito da x
MovingObject::Collision MovingObject::CheckCollision(GameObject* other)
{
    Hitbox thisHitbox = this->GetHitbox();
    Hitbox otherHitbox = other->GetHitbox();

    //Calcola poligono di soprapposizione mesh: overlapX = min(max1.x,max2.x) - max(min1.x,min2.x), overlapY = ...
    float overlapX = std::min(thisHitbox.Max.x, otherHitbox.Max.x) - std::max(thisHitbox.Min.x, otherHitbox.Min.x);
    float overlapY = std::min(thisHitbox.Max.y, otherHitbox.Max.y) - std::max(thisHitbox.Min.y, otherHitbox.Min.y);

    if (overlapX <= 0 || overlapY <= 0)
        return Collision::None;

    if (overlapX < overlapY) // Valuta l'asse X
    {
        if (this->Position.x < other->Position.x)
            return Collision::Right;
        else
            return Collision::Left;
    }
    else // Valuta l'asse Y
    {
        if (this->Position.y < other->Position.y)
            return Collision::Top;
        else
            return Collision::Bottom;
    }
}
void MovingObject::CheckCollisionWithSolids(const std::vector<GameObject*>& solidObjects)
{
    this->isOnGround = false;   // Considero l'oggetto a mezz'aria prima del controllo delle collisioni

    for (GameObject* solid : solidObjects)
    {  
        Collision collision = CheckCollision(solid);
        if (collision != Collision::None)
        {
            HandleCollisionWithSolid(solid, collision);
        }
    }
}
void MovingObject::HandleCollisionWithSolid(GameObject* solidObject, Collision collision)
{
    /*
    Hitbox thisHitbox = this->GetHitbox();
    Hitbox solidHitbox = solidObject->GetHitbox();

    float overlapX = std::min(thisHitbox.Max.x, solidHitbox.Max.x) - std::max(thisHitbox.Min.x, solidHitbox.Min.x);
    float overlapY = std::min(thisHitbox.Max.y, solidHitbox.Max.y) - std::max(thisHitbox.Min.y, solidHitbox.Min.y);

    // Correggo l'overlap minore
    if (overlapX < overlapY) // Correggo sull'asse X
    {
        if (this->Position.x < solidObject->Position.x) // Collisione a destra del MovingObject
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
        if (this->Position.y < solidObject->Position.y) // Collisione sopra del MovingObject
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
    */

    Hitbox thisHitbox = this->GetHitbox();
    Hitbox otherHitbox = solidObject->GetHitbox();

    float overlapX = std::min(thisHitbox.Max.x, otherHitbox.Max.x) - std::max(thisHitbox.Min.x, otherHitbox.Min.x);
    float overlapY = std::min(thisHitbox.Max.y, otherHitbox.Max.y) - std::max(thisHitbox.Min.y, otherHitbox.Min.y);

    switch (collision) {
        case Collision::Right:  this->Position.x -= overlapX;
                                break;
        case Collision::Left:   this->Position.x += overlapX;
                                break;
        case Collision::Top:    this->Position.y -= overlapY;
                                //this->velocity.y = 0;
                                break;
        case Collision::Bottom: this->Position.y += overlapY;
                                this->isOnGround = true;
                                //this->velocity.y = 0;
                                break;
        default: {}
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
        model_mat = glm::rotate(model_mat, glm::radians(this->Rotation), Axis);
        if (usePivot) {
            model_mat = glm::translate(model_mat, glm::vec3(this->Pivot, 0.0f));
            model_mat = glm::rotate(model_mat, glm::radians(this->Rotation), Axis);
            model_mat = glm::translate(model_mat, glm::vec3(-this->Pivot, 0.0f));
        }
        else {
            model_mat = glm::rotate(model_mat, glm::radians(this->Rotation), Axis);
        }
        //model_mat = glm::scale(model_mat, this->Size);
        model_mat = glm::scale(model_mat, glm::vec3(FlipX * this->Size.x, this->Size.y, 1.0f));

        GameObject::Render(Shader, model_mat);
    }

    // Se l'oggetto si trova oltre il bordo a sinistra renderizzo la parte mancante a destra
    if (this->Position.x <= -1.0f + this->Size.x / 2)
    {
        glm::mat4 model_mat = glm::mat4(1.0f);
        model_mat = glm::translate(model_mat, glm::vec3((this->Position + glm::vec2(2.0f, 0.0f)), 0.0f));
        if (usePivot) {
            model_mat = glm::translate(model_mat, glm::vec3(this->Pivot, 0.0f));
            model_mat = glm::rotate(model_mat, glm::radians(this->Rotation), Axis);
            model_mat = glm::translate(model_mat, glm::vec3(-this->Pivot, 0.0f));
        }
        else {
            model_mat = glm::rotate(model_mat, glm::radians(this->Rotation), Axis);
        }
        //model_mat = glm::scale(model_mat, this->Size);
        model_mat = glm::scale(model_mat, glm::vec3(FlipX * this->Size.x, this->Size.y, 1.0f));

        GameObject::Render(Shader, model_mat);
    }
}