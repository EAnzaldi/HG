#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "stb_image.h"
#include "shader_s.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include "TextureObject.h"
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
    glm::vec2 Size;
    float Rotation;

    TextureObject Texture;

    GameObject(glm::vec2 position, glm::vec2 size, TextureObject texture, bool repeatWidth);

    void Render(const Shader& shader) const;

    Hitbox GetHitbox() const;

private:
    unsigned int VAO;

    void initRenderData(bool repeatWidth);

};