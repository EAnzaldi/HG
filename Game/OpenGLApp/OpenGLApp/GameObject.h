#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "stb_image.h"
#include "shader_s.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include "TextureObject.h"
#include "model.h"
#include "constants.h"
#include <vector>

struct Hitbox 
{
    glm::vec2 Min; // Punto in basso a sinistra
    glm::vec2 Max; // Punto in alto a destra
};

class GameObject 
{
public:
    glm::vec2 Position;
    glm::vec3 Size;
    float Rotation;
    float FlipX;

    bool RepeatWidth;

    Model model;

    TextureObject* Texture;

    GameObject(glm::vec2 position, glm::vec3 size, Model model, TextureObject* texture, bool repeatWidth);

    virtual void Render(const Shader& shader) const;

    Hitbox GetHitbox() const;

};